using Common;
using Common.Bindings;
using DeviceManager.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace DeviceManager.ViewModel
{
    public class OpticsViewModel : BindableBase
    {
        private IOpticsModel opticsModel;
        private string selectedLed;
        private string selectedPhotodiode;
        private int integrationTime;
        private int intensity;
        private const int updateDelay = 300;
        private float photodiodeVolts;
        private int photodiodeRaw;
        private string statusMessage;

        public OpticsViewModel(IOpticsModel opticsModel)
        {
            this.opticsModel = opticsModel;

            SetIntegrationTimeCommand = new RelayCommand(param => UpdateIntegrationTime());
            SetIntensityCommand = new RelayCommand(param => UpdateIntensity());

            // Initial Set
            IntegrationTime = OpticsDefault.IntegrationTimeDefault;
            Intensity = 0;
            Leds = OpticsDefault.Leds;
            SelectedLed = Leds[0];
            Photodiodes = OpticsDefault.Photodiodes;
            SelectedPhotodiode = Photodiodes[0];

            InitialUpdate();
            StartUpdateTask();
        }

        private void InitialUpdate()
        {
            var status = opticsModel.ReadStatusCommand().Result;
            if (status.succesfulResponse)
            {
                ProcessStatus(status.response);
            }

            var photodiodeRawRead = opticsModel.ReadPhotodiodeRawCommand().Result;
            if (photodiodeRawRead.succesfulResponse)
            {
                PhotodiodeRaw = Helper.GetIntFromBigEndian(photodiodeRawRead.response);
            }

            var photodiodeVoltsRead = opticsModel.ReadPhotodiodeVoltsCommand().Result;
            if (photodiodeVoltsRead.succesfulResponse)
            {
                PhotodiodeVolts = Helper.GetFloatFromBigEndian(photodiodeVoltsRead.response);
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
                var status = await opticsModel.ReadStatusCommand();
                if (status.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        //VSense = Helper.GetFloatFromBigEndian(vData.response);
                        ProcessStatus(status.response);
                    }));
                }

                Thread.Sleep(updateDelay);

                var photodiodeRawRead = await opticsModel.ReadPhotodiodeRawCommand();
                if (photodiodeRawRead.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        PhotodiodeRaw = Helper.GetIntFromBigEndian(photodiodeRawRead.response);
                    }));
                }

                Thread.Sleep(updateDelay);

                var photodiodeVoltsRead = await opticsModel.ReadPhotodiodeVoltsCommand();
                if (photodiodeVoltsRead.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        PhotodiodeVolts = Helper.GetFloatFromBigEndian(photodiodeVoltsRead.response);
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
