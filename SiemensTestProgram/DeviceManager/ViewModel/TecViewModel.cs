// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common;
    using Common.Bindings;
    using Model;
    using System;
    using System.Collections.Generic;
    using System.Threading;
    using System.Threading.Tasks;

    public class TecViewModel : BindableBase
    {
        private object mutex = new object();
        private Task updateTask;
        private ITecModel tecModel;
        private int progressMaximum;
        private string selectedWaveForm;
        private string saveProgress;
        private string enableButtonState;
        private string captureButtonState;
        private int saveProgressValue;
        private int numberOfSamples;
        private float voltageValue;
        private int sliderVoltageValue;
        private int tecPeriod;

        private float vSense;
        private float iSense;
        private float iRef;
        private string statusMessage;

        private const string StartCaptureText = "Start Capture";
        private const string StopCaptureText = "Stop Capture";
        private const string EnableText = "Enable";
        private const string DisableText = "Disable";
        private const int updateDelay = 400;
        private bool saving;
        private bool updating;

        CancellationTokenSource cts;
        CancellationToken token;

        public TecViewModel(ITecModel tecModel)
        {
            this.tecModel = tecModel;

            // Initial values
            progressMaximum = 100;
            tecPeriod = 1000;
            Waveforms = TecDefaults.Waveforms;
            selectedWaveForm = Waveforms[0];
            saveProgressValue = 0;
            saveProgress = string.Empty;
            captureButtonState = StartCaptureText;
            enableButtonState = EnableText;
            numberOfSamples = 0;
            SliderVoltageValue = 250;

            // Set commands
            SaveDataCommand = new RelayCommand(param => SaveData());
            EnableCommand = new RelayCommand(param => EnableToggle());
            CaptureStartStopCommand = new RelayCommand(param => CaptureToggle());
            ResetTecCommand = new RelayCommand(param => ResetTec());

            StartUpdateTask();
        }

        public RelayCommand SaveDataCommand { get; set; }

        public RelayCommand EnableCommand { get; set; }

        public RelayCommand ResetTecCommand { get; set; }

        public RelayCommand CaptureStartStopCommand { get; set; }

        public List<string> Waveforms { get; set; }

        public string EnableButtonState
        {
            get
            {
                return enableButtonState;
            }

            set
            {
                enableButtonState = value;
                OnPropertyChanged(nameof(EnableButtonState));
            }
        }

        public string CaptureButtonState
        {
            get
            {
                return captureButtonState;
            }

            set
            {
                captureButtonState = value;
                OnPropertyChanged(nameof(CaptureButtonState));
            }
        }

        public int TecPeriod
        {
            get
            {
                return tecPeriod;
            }
            set
            {
                if (value < TecDefaults.PeriodMinimum)
                {
                    tecPeriod = TecDefaults.PeriodMinimum;
                }
                else if (value > TecDefaults.MaximumNumberOfSamples)
                {
                    tecPeriod = TecDefaults.MaximumNumberOfSamples;
                }
                else
                {
                    tecPeriod = value;
                }

                OnPropertyChanged(nameof(TecPeriod));
                UpdatePeriod();
            }
        }

        public int NumberOfSamples
        {
            get
            {
                return numberOfSamples;
            }
            set
            {
                if (value < 0)
                {
                    numberOfSamples = 0;
                }
                else if (value > TecDefaults.MaximumNumberOfSamples)
                {
                    numberOfSamples = TecDefaults.MaximumNumberOfSamples;
                }
                else
                {
                    numberOfSamples = value;
                }
                
                OnPropertyChanged(nameof(NumberOfSamples));
            }
        }

        public string SelectedWaveForm
        {
            get
            {
                return selectedWaveForm;
            }

            set
            {
                selectedWaveForm = value;
                OnPropertyChanged(nameof(SelectedWaveForm));
                UpdateWaveform();
            }
        }

        public string SaveProgress
        {
            get
            {
                return saveProgress;
            }

            set
            {
                saveProgress = value;
                OnPropertyChanged(nameof(SaveProgress));
            }
        }

        public int SaveProgressValue
        {
            get
            {
                return saveProgressValue;
            }

            set
            {
                saveProgressValue = value;
                OnPropertyChanged(nameof(SaveProgressValue));
            }
        }

        public int ProgressMaximum
        {
            get
            {
                return progressMaximum;
            }

            set
            {
                progressMaximum = value;
                OnPropertyChanged(nameof(ProgressMaximum));
            }
        }

        public float VoltageValue
        {
            get
            {
                return voltageValue;
            }

            set
            {
                voltageValue = value;
                OnPropertyChanged(nameof(VoltageValue));
                OnPropertyChanged(nameof(VoltageValueText));
            }
        }

        public string VoltageValueText
        {
            get
            {
                return $"{VoltageValue} V";
            }
        }

        public string IRefText
        {
            get
            {
                return $"IRef: {iRef}";
            }
        }

        public float IRef
        {
            get
            {
                return iRef;
            }

            set
            {
                iRef = value;
                OnPropertyChanged(nameof(IRef));
                OnPropertyChanged(nameof(IRefText));
            }
        }

        public string VSenseText
        {
            get
            {
                return $"VSense: {vSense}";
            }
        }

        public float VSense
        {
            get
            {
                return vSense;
            }

            set
            {
                vSense = value;
                OnPropertyChanged(nameof(VSense));
                OnPropertyChanged(nameof(VSenseText));
            }
        }

        public string ISenseText
        {
            get
            {
                return $"ISense: {iSense}";
            }
        }

        public float ISense
        {
            get
            {
                return iSense;
            }

            set
            {
                iSense = value;
                OnPropertyChanged(nameof(ISense));
                OnPropertyChanged(nameof(ISenseText));
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

        public int SliderVoltageValue
        {
            get
            {
                return sliderVoltageValue;
            }
            set
            {
                sliderVoltageValue = value;
                VoltageValue = (float)sliderVoltageValue / 100;
                OnPropertyChanged(nameof(SliderVoltageValue));
            }
        }

        private void SaveData()
        {
            saving = true;
            cts.Cancel();

            // Wait to finish update
            updateTask.Wait();
            StatusMessage = "Saving";

            SaveProgressValue += 10;
            SaveProgress = FormatSaveProgress();
            saving = false;
            // uncomment later StartUpdateTask();
        }

        private async void EnableToggle()
        {
            EnableButtonState = enableButtonState == EnableText ? DisableText : EnableText;
            var status = await tecModel.ControlCommand(enableButtonState);
            ProcessStatus(status);
        }

        private async void UpdatePeriod()
        {
            var status = await tecModel.SetPeriodCommand(tecPeriod);
            ProcessStatus(status);
        }

        private async void UpdateWaveform()
        {
            var status = await tecModel.SetWaveformCommand(selectedWaveForm);
            ProcessStatus(status);
        }

        private void CaptureToggle()
        {
            CaptureButtonState = captureButtonState == StartCaptureText ? StopCaptureText : StartCaptureText;
        }
        
        private void ResetTec()
        {
            // set waveform to continuous, samples = ?, period 1000ms
        }

        private async void UpdateIref()
        {
            var data = await tecModel.ReadIref();
            IRef = Helper.GetFloatFromBigEndian(data);
        }

        private async void UpdateISense()
        {
            var data = await tecModel.ReadIsense();
            ISense = Helper.GetFloatFromBigEndian(data);
        }

        private async void UpdateVSense()
        {
            var data = await tecModel.ReadVsense();
            VSense = Helper.GetFloatFromBigEndian(data); 
        }

        private async void UpdateStatus()
        {
            var status = await tecModel.ReadStatus();
            ProcessStatus(status);
        }

        private void ProcessStatus(byte[] status)
        {
            if (status.Length < 4)
            {
                statusMessage = "Response length incorrect";
                return;
            }

            StatusMessage = GetErrorMessage(status[4]);
        }

        private void StartUpdateTask()
        {
            cts = new CancellationTokenSource();
            token = cts.Token;

            updateTask = Task.Factory.StartNew(() =>
            {
                updating = true;
                UpdateAllStatuses();
            }, token);
        }

        private async void UpdateAllStatuses()
        {
            while (true)
            {
                if (token.IsCancellationRequested == true)
                {
                    updating = false;
                    break;
                }

                try
                {
                    var iRefData = await tecModel.ReadIref();
                    IRef = Helper.GetFloatFromBigEndian(iRefData);
                    Thread.Sleep(updateDelay);
                    var iSenseData = await tecModel.ReadIsense();
                    ISense = Helper.GetFloatFromBigEndian(iSenseData);
                    Thread.Sleep(updateDelay);
                    var vSenseData = await tecModel.ReadVsense();
                    VSense = Helper.GetFloatFromBigEndian(vSenseData);
                    Thread.Sleep(updateDelay);
                    var status = await tecModel.ReadStatus();
                    ProcessStatus(status);
                    Thread.Sleep(updateDelay);
                }
                catch (Exception e)
                {
                    StatusMessage = e.Message;
                }
                
                
            }
        }

        private string FormatSaveProgress()
        {
            return $"{SaveProgressValue} / {ProgressMaximum}";
        }

        private string GetErrorMessage(byte value)
        {
            string response;
            var s = TecDefaults.Errors.TryGetValue(value, out response);

            return response == null ? "Unknown" : response;
        }
    }
}
