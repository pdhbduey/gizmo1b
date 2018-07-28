namespace DeviceManager.ViewModel
{
    using Common;
    using Common.Bindings;
    using DeviceManager.Model;
    using System;
    using System.Collections.Generic;
    using System.Threading;
    using System.Windows;

    public class OpticsViewModel : BindableBase
    {
        private IOpticsModel opticsModel;
        private string selectedLed;
        private string selectedPhotodiode;
        private int integrationTime;
        private int intensity;
        private const int updateDelay = 300;
        private float photodiodeVolts;
        private float pdTemperature;
        private float ledTemperature;
        private float pdTemperatureDuringIntegration;
        private float ledTemperatureDuringIntegration;
        private int photodiodeRaw;
        private int selectedPdVersion;
        private int selectedLedVersion;
        private float ledMonitorVolts;
        private string statusMessage;
        private bool ledBoardEnabled;
        private bool pdBoardEnabled;

        public OpticsViewModel(IOpticsModel opticsModel)
        {
            this.opticsModel = opticsModel;

            SetIntegrationTimeCommand = new RelayCommand(param => UpdateIntegrationTime());
            SetIntensityCommand = new RelayCommand(param => UpdateIntensity());
            UpdateCommand = new RelayCommand(param => Update());

            // Initial Set
            IntegrationTime = OpticsDefault.IntegrationTimeDefault;
            Intensity = 0;
            Leds = OpticsDefault.Leds;
            SelectedLed = Leds[0];
            Photodiodes = OpticsDefault.Photodiodes;
            SelectedPhotodiode = Photodiodes[0];

            PdBoardVersions = OpticsDefault.PdBoardVersions;
            SelectedPdVersion = PdBoardVersions[0];
            LedBoardVersions = OpticsDefault.LedBoardVersions;
            SelectedLedVersion = LedBoardVersions[0];

            ledBoardEnabled = false;
            pdBoardEnabled = false;
            SetToDefaultControlSettings();

            Update();
            StartUpdateTask();
        }

        private void SetToDefaultControlSettings()
        {
            opticsModel.ResetControlSettingsCommand().Wait();
        }

        private async void SetLedBoardEnabledControl()
        {
            if (ledBoardEnabled)
            {
                await opticsModel.LedBoardEnableCommand();
                return;
            }

            await opticsModel.LedBoardDisableCommand();
        }

        private async void SetPdBoardEnabledControl()
        {
            if (pdBoardEnabled)
            {
                await opticsModel.PdBoardEnableCommand();
                return;
            }

            await opticsModel.PdBoardDisableCommand();
        }

        public bool LedBoardEnabled
        {
            get
            {
                return ledBoardEnabled;
            }

            set
            {
                ledBoardEnabled = value;
                SetLedBoardEnabledControl();
            }
        }

        public bool PdBoardEnabled
        {
            get
            {
                return pdBoardEnabled;
            }

            set
            {
                pdBoardEnabled = value;
                SetPdBoardEnabledControl();
            }
        }

        public RelayCommand UpdateCommand { get; set; }

        private void Update()
        {
            var photodiodeVoltsRead = opticsModel.ReadPhotodiodeVoltsCommand().Result;
            if (photodiodeVoltsRead.succesfulResponse)
            {
                PhotodiodeVolts = Helper.GetFloatFromBigEndian(photodiodeVoltsRead.response);
            }

            var photodiodeRawRead = opticsModel.ReadPhotodiodeRawCommand().Result;
            if (photodiodeRawRead.succesfulResponse)
            {
                PhotodiodeRaw = Helper.GetIntFromBigEndian(photodiodeRawRead.response);
            }

            var readLedMonitor = opticsModel.ReadLedMonitorVolts().Result;
            if (readLedMonitor.succesfulResponse)
            {
                LedMonitorVolts = Helper.GetFloatFromBigEndian(readLedMonitor.response);
            }

            var ledTemperatureReadDuringIntegration = opticsModel.ReadLedTemperatureDuringIntegrationCommand().Result;
            if (ledTemperatureReadDuringIntegration.succesfulResponse)
            {
                LedTemperatureDuringIntegration = Helper.GetFloatFromBigEndian(ledTemperatureReadDuringIntegration.response);
            }

            var pdTemperatureReadDuringIntegration = opticsModel.ReadPdTemperatureDuringIntegrationCommand().Result;
            if (pdTemperatureReadDuringIntegration.succesfulResponse)
            {
                PdTemperatureDuringIntegration = Helper.GetFloatFromBigEndian(pdTemperatureReadDuringIntegration.response);
            }

            var ledTemperatureRead = opticsModel.ReadLedTemperatureCommand().Result;
            if (ledTemperatureRead.succesfulResponse)
            {
                LedTemperature = Helper.GetFloatFromBigEndian(ledTemperatureRead.response);
            }

            var pdTemperatureRead = opticsModel.ReadPdTemperatureCommand().Result;
            if (pdTemperatureRead.succesfulResponse)
            {
                PdTemperature = Helper.GetFloatFromBigEndian(pdTemperatureRead.response);
            }

            var status = opticsModel.ReadStatusCommand().Result;
            if (status.succesfulResponse)
            {
                ProcessStatus(status.response);
            }
        }

        private void StartUpdateTask()
        {
            var thread = new Thread(() =>
            {
                UpdateAllStatuses();
            });

            thread.SetApartmentState(ApartmentState.STA);
            thread.Start();
        }

        private async void UpdateAllStatuses()
        {
            while (true)
            {
                var pdTemperatureRead = await opticsModel.ReadPdTemperatureCommand();
                if (pdTemperatureRead.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        PdTemperature = Helper.GetFloatFromBigEndian(pdTemperatureRead.response);
                    }));
                }

                Thread.Sleep(updateDelay);

                var ledTemperatureRead = await opticsModel.ReadLedTemperatureCommand();
                if (ledTemperatureRead.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        LedTemperature = Helper.GetFloatFromBigEndian(ledTemperatureRead.response);
                    }));
                }

                Thread.Sleep(updateDelay);

                var status = await opticsModel.ReadStatusCommand();
                if (status.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        ProcessStatus(status.response);
                    }));
                }

                Thread.Sleep(updateDelay);
            }
        }

        private void ProcessStatus(byte[] status)
        {
            if (status.Length < 4)
            {
                StatusMessage = "Communication Error";
                return;
            }

            string value;
            if (OpticsDefault.StatusValues.TryGetValue(status[3], out value))
            {
                StatusMessage = value;
            }
        }

        public string StatusMessage
        {
            get
            {
                return $"Status: {statusMessage}";
            }

            set
            {
                statusMessage = value;
                OnPropertyChanged(nameof(StatusMessage));
            }
        }

        public string LedMonitorVoltsText
        {
            get
            {
                return $"LED monitor during integrating: {ledMonitorVolts.ToString("0.##")} V";
            }
        }

        public float LedMonitorVolts
        {
            get
            {
                return ledMonitorVolts;
            }

            set
            {
                ledMonitorVolts = value;
                OnPropertyChanged(nameof(LedMonitorVoltsText));
            }
        }

        public string PdTemperatureText
        {
            get
            {
                return $"Photodiode temperature: {pdTemperature.ToString("0.##")} °C";
            }
        }

        public string LedTemperatureText
        {
            get
            {
                return $"LED temperature: {ledTemperature.ToString("0.##")} °C";
            }
        }

        public float PdTemperature
        {
            get
            {
                return pdTemperature;
            }

            set
            {
                pdTemperature = value;
                OnPropertyChanged(nameof(PdTemperatureText));
            }
        }

        public float LedTemperature
        {
            get
            {
                return ledTemperature;
            }

            set
            {
                ledTemperature = value;
                OnPropertyChanged(nameof(LedTemperatureText));
            }
        }

        public string PdTemperatureDuringIntegrationText
        {
            get
            {
                return $"Photodiode temperature during integration: {pdTemperatureDuringIntegration.ToString("0.##")} °C";
            }
        }

        public string LedTemperatureDuringIntegrationText
        {
            get
            {
                return $"LED temperature during integration: {ledTemperatureDuringIntegration.ToString("0.##")} °C";
            }
        }

        public float PdTemperatureDuringIntegration
        {
            get
            {
                return pdTemperatureDuringIntegration;
            }

            set
            {
                pdTemperatureDuringIntegration = value;
                OnPropertyChanged(nameof(PdTemperatureDuringIntegrationText));
            }
        }

        public float LedTemperatureDuringIntegration
        {
            get
            {
                return ledTemperatureDuringIntegration;
            }

            set
            {
                ledTemperatureDuringIntegration = value;
                OnPropertyChanged(nameof(LedTemperatureDuringIntegrationText));
            }
        }

        public string PhotodiodeVoltsText
        {
            get
            {
                return $"Photodiode Voltage: {photodiodeVolts.ToString("0.##")} V";
            }
        }

        public string PhotodiodeRawText
        {
            get
            {
                return $"Photodiode Raw: {photodiodeRaw.ToString()}";
            }
        }

        private float PhotodiodeVolts
        {
            get
            {
                return photodiodeVolts;
            }

            set
            {
                photodiodeVolts = value;
                OnPropertyChanged(nameof(PhotodiodeVoltsText));
            }
        }



        private int PhotodiodeRaw
        {
            get
            {
                return photodiodeRaw;
            }

            set
            {
                photodiodeRaw = value;
                OnPropertyChanged(nameof(PhotodiodeRawText));
            }
        }

        private async void UpdateIntegrationTime()
        {
            if (integrationTime < OpticsDefault.IntegrationTimeMinimum)
            {
                IntegrationTime = OpticsDefault.IntegrationTimeMinimum;
            }
            else if (integrationTime > OpticsDefault.IntegrationTimeMaximum)
            {
                IntegrationTime = OpticsDefault.IntegrationTimeMaximum;
            }

            await opticsModel.SetIntegrationTimeCommand(integrationTime);
        }

        private async void UpdateIntensity()
        {
            if (intensity < OpticsDefault.IntensityMinimum)
            {
                Intensity = OpticsDefault.IntensityMinimum;
            }
            else if (intensity > OpticsDefault.IntensityMaximum)
            {
                Intensity = OpticsDefault.IntensityMaximum;
            }

            await opticsModel.SetIntensityCommand(intensity);
        }

        public RelayCommand SetIntegrationTimeCommand { get; set; }

        public RelayCommand SetIntensityCommand { get; set; }

        public int Intensity
        {
            get
            {
                return intensity;
            }
            set
            {
                intensity = value;

                OnPropertyChanged(nameof(Intensity));
            }
        }

        public int IntegrationTime
        {
            get
            {
                return integrationTime;
            }
            set
            {
                integrationTime = value;

                OnPropertyChanged(nameof(IntegrationTime));
            }
        }

        public List<int> PdBoardVersions { get; set; }

        public int SelectedPdVersion
        {
            get
            {
                return selectedPdVersion;
            }

            set
            {
                selectedPdVersion = value;
                OnPropertyChanged(nameof(SelectedPdVersion));
                UpdatePdBoardVersion();
            }
        }

        private async void UpdatePdBoardVersion()
        {
            await opticsModel.SetPdBoardVersion(selectedPdVersion);
        }

        public List<int> LedBoardVersions { get; set; }

        public int SelectedLedVersion
        {
            get
            {
                return selectedLedVersion;
            }

            set
            {
                selectedLedVersion = value;
                OnPropertyChanged(nameof(SelectedLedVersion));
                ULedateLedBoardVersion();
            }
        }

        private async void ULedateLedBoardVersion()
        {
            await opticsModel.SetLedBoardVersion(selectedLedVersion);
        }

        public List<string> Leds { get; set; }

        public string SelectedLed
        {
            get
            {
                return selectedLed;
            }

            set
            {
                selectedLed = value;
                OnPropertyChanged(nameof(SelectedLed));
                UpdateLed();
            }
        }

        private async void UpdateLed()
        {
            await opticsModel.SetLedCommand(selectedLed);
        }

        public List<string> Photodiodes { get; set; }

        public string SelectedPhotodiode
        {
            get
            {
                return selectedPhotodiode;
            }

            set
            {
                selectedPhotodiode = value;
                OnPropertyChanged(nameof(SelectedPhotodiode));
                UpdatePhotodiode();
            }
        }

        private async void UpdatePhotodiode()
        {
            await opticsModel.SetPhotodiodeCommand(selectedPhotodiode);
        }


    }
}
