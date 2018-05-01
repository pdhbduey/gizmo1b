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
        private Task updateTask;
        private ITecModel tecModel;
        private int progressMaximum;
        private string selectedWaveForm;
        private string saveProgress;
        private string enableButtonState;
        private string captureButtonState;
        private string waveformButtonState;
        private string closedLoopButtonState;
        private int saveProgressValue;
        private int numberOfSamples;
        private float irefCurrentValue;
        private int sliderIrefValue;
        //private int irefGain;
        private int tecPeriod;
        private int counter;
        private int waveformCycles;
        private int sampleTime;

        private float vSense;
        private float iSense;
        private float iRef;
        private float proportionalGain;
        private int integralGain;
        private int derivativeGain;
        private string statusMessage;

        private const int updateDelay = 300;
        private bool saving;
        private bool updating;

        CancellationTokenSource cts;
        CancellationToken token;

        public TecViewModel(ITecModel tecModel)
        {
            this.tecModel = tecModel;

            // Initial values
            saving = false;
            updating = false;
            counter = 0;
            progressMaximum = 100;
            tecPeriod = TecDefaults.PeriodMinimum;
            waveformCycles = TecDefaults.WaveformCyclesMinimum;
            sampleTime = TecDefaults.SampleTimeMinimum;
            //irefGain = TecDefaults.IrefGainMinimum;
            Waveforms = TecDefaults.Waveforms;
            selectedWaveForm = Waveforms[0];
            saveProgressValue = 0;
            saveProgress = string.Empty;
            captureButtonState = TecDefaults.StartCaptureText;
            waveformButtonState = TecDefaults.StartWaveformText;
            enableButtonState = TecDefaults.EnableText;
            closedLoopButtonState = TecDefaults.EnableClosedLoopText;
            numberOfSamples = 0;

            InitialUpdate();
            //SliderIrefValue = 0;

            // Set commands
            SaveDataCommand = new RelayCommand(param => SaveData());
            EnableCommand = new RelayCommand(param => EnableToggle());
            CaptureStartStopCommand = new RelayCommand(param => CaptureToggle());
            StartStopWaveformCommand = new RelayCommand(param => WaveformToggle());
            ClosedLoopToggleCommand = new RelayCommand(param => ClosedLoopToggle());
            ResetTecCommand = new RelayCommand(param => ResetTec());
            ResetCustomWaveformCommand = new RelayCommand(param => Reset());
            UpdateIrefCommand = new RelayCommand(param => UpdateIrefForWaveform());
            IncrementCommand = new RelayCommand(param => IncrementCounter());

            StartUpdateTask();
        }

        public RelayCommand SaveDataCommand { get; set; }

        public RelayCommand EnableCommand { get; set; }

        public RelayCommand ResetTecCommand { get; set; }

        public RelayCommand ResetCustomWaveformCommand { get; set; }

        public RelayCommand CaptureStartStopCommand { get; set; }

        public RelayCommand StartStopWaveformCommand { get; set; }

        public RelayCommand ClosedLoopToggleCommand { get; set; }

        public RelayCommand UpdateIrefCommand { get; set; }

        public RelayCommand IncrementCommand { get; set; }

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

        public int Counter
        {
            get
            {
                return counter;
            }

            set
            {
                counter = value;
                OnPropertyChanged(nameof(Counter));
            }
        }

        public string ClosedLoopButtonState
        {
            get
            {
                return closedLoopButtonState;
            }

            set
            {
                closedLoopButtonState = value;
                OnPropertyChanged(nameof(ClosedLoopButtonState));
            }
        }

        public bool IsIRefEditable
        {
            get
            {
                return (waveformButtonState == TecDefaults.StartWaveformText);
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

        public string WaveformButtonState
        {
            get
            {
                return waveformButtonState;
            }

            set
            {
                waveformButtonState = value;
                OnPropertyChanged(nameof(WaveformButtonState));
                OnPropertyChanged(nameof(IsIRefEditable));
            }
        }

        public float ProportionalGain
        {
            get
            {
                return proportionalGain;
            }
            set
            {
                if (value < TecDefaults.ProportionalGainMinimum)
                {
                    proportionalGain = TecDefaults.ProportionalGainMinimum;
                }
                else if (value > TecDefaults.ProportionalGainMaximum)
                {
                    proportionalGain = TecDefaults.ProportionalGainMaximum;
                }
                else
                {
                    proportionalGain = value;
                }

                OnPropertyChanged(nameof(ProportionalGain));
                UpdateProportionalGain();
            }
        }

        public int IntegralGain
        {
            get
            {
                return integralGain;
            }
            set
            {
                if (value < TecDefaults.IntegralGainMinimum)
                {
                    integralGain = TecDefaults.IntegralGainMinimum;
                }
                else if (value > TecDefaults.IntegralGainMaximum)
                {
                    integralGain = TecDefaults.IntegralGainMaximum;
                }
                else
                {
                    integralGain = value;
                }

                OnPropertyChanged(nameof(IntegralGain));
                UpdateIntegralGain();
            }
        }

        public int DerivativeGain
        {
            get
            {
                return derivativeGain;
            }
            set
            {
                if (value < TecDefaults.DerivativeGainMinimum)
                {
                    derivativeGain = TecDefaults.DerivativeGainMinimum;
                }
                else if (value > TecDefaults.DerivativeGainMaximum)
                {
                    derivativeGain = TecDefaults.DerivativeGainMaximum;
                }
                else
                {
                    derivativeGain = value;
                }

                OnPropertyChanged(nameof(DerivativeGain));
                UpdateDerivativeGain();
            }
        }

        public int WaveformCycles
        {
            get
            {
                return waveformCycles;
            }

            set
            {
                if (value < TecDefaults.WaveformCyclesMinimum)
                {
                    waveformCycles = TecDefaults.WaveformCyclesMinimum;
                }
                else if (value > TecDefaults.WaveformCyclesMaximum)
                {
                    waveformCycles = TecDefaults.WaveformCyclesMaximum;
                }
                else
                {
                    waveformCycles = value;
                }
                OnPropertyChanged(nameof(WaveformCycles));

                SetWaveformCycles();
            }
        }

        public int SampleTime
        {
            get
            {
                return sampleTime;
            }

            set
            {
                if (value < TecDefaults.SampleTimeMinimum)
                {
                    sampleTime = TecDefaults.SampleTimeMinimum;
                }
                else if (value > TecDefaults.SampleTimeMaximum)
                {
                    sampleTime = TecDefaults.SampleTimeMaximum;
                }
                else
                {
                    sampleTime = value;
                }
                OnPropertyChanged(nameof(SampleTime));

                SetSampleTime();
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
                else if (value > TecDefaults.PeriodMaximum)
                {
                    tecPeriod = TecDefaults.PeriodMaximum;
                }
                else
                {
                    tecPeriod = value;
                }

                OnPropertyChanged(nameof(TecPeriod));
                UpdatePeriod();
            }
        }

        //public int IRefGain
        //{
        //    get
        //    {
        //        return irefGain;
        //    }
        //    set
        //    {
        //        if (value < TecDefaults.IrefGainMinimum)
        //        {
        //            irefGain = TecDefaults.IrefGainMinimum;
        //        }
        //        else if (value > TecDefaults.IrefGainMaximum)
        //        {
        //            irefGain = TecDefaults.IrefGainMaximum;
        //        }
        //        else
        //        {
        //            irefGain = value;
        //        }

        //        OnPropertyChanged(nameof(IRefGain));
        //        UpdateIrefGain();
        //    }
        //}

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

        public float IrefCurrentValue
        {
            get
            {
                return irefCurrentValue;
            }

            set
            {
                irefCurrentValue = value; 
                OnPropertyChanged(nameof(IrefCurrentValue));

                sliderIrefValue = (int)irefCurrentValue * 100;
                OnPropertyChanged(nameof(SliderIrefValue));
            }
        }

        //public string IrefCurrentValueText
        //{
        //    get
        //    {
        //        return $"{IrefCurrentValue.ToString("0.##")} A";
        //    }
        //}

        public string IRefText
        {
            get
            {
                return $"IRef: {iRef.ToString("0.##")} A";
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
                return $"VSense: {vSense.ToString("0.##")} V";
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
                return $"ISense: {iSense.ToString("0.##")} A";
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

        public int SliderIrefValue
        {
            get
            {
                return sliderIrefValue;
            }
            set
            {
                sliderIrefValue = value;
                IrefCurrentValue = (float)sliderIrefValue / 100;
                OnPropertyChanged(nameof(SliderIrefValue));

                // Send Iref command
                UpdateIrefForWaveform();
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
            var state = enableButtonState;
            EnableButtonState = enableButtonState == TecDefaults.EnableText ? TecDefaults.DisableText : TecDefaults.EnableText;
            var status = await tecModel.ControlCommand(state);
        }

        private async void UpdatePeriod()
        {
            var status = await tecModel.SetPeriodCommand(tecPeriod);
        }

        private async void UpdateProportionalGain()
        {
            var status = await tecModel.SetProportionalGainCommand(proportionalGain);
        }

        private async void UpdateIntegralGain()
        {
            var status = await tecModel.SetIntegralGainCommand(integralGain);
        }

        private async void UpdateDerivativeGain()
        {
            var status = await tecModel.SetDerivativeGainCommand(derivativeGain);
        }

        private async void UpdateIrefForWaveform()
        {
            var status = await tecModel.SetIrefCommand(irefCurrentValue);
        }

        private async void UpdateWaveform()
        {
            var status = await tecModel.SetWaveformCommand(selectedWaveForm);
        }

        private void CaptureToggle()
        {
            CaptureButtonState = captureButtonState == TecDefaults.StartCaptureText ? TecDefaults.StopCaptureText : TecDefaults.StartCaptureText;
        }

        private async void WaveformToggle()
        {
            var state = waveformButtonState;
            WaveformButtonState = waveformButtonState == TecDefaults.StartWaveformText ? TecDefaults.StopWaveformText : TecDefaults.StartWaveformText;
            var status = await tecModel.ControlCommand(state);
        }

        private async void ClosedLoopToggle()
        {
            var state = closedLoopButtonState;
            ClosedLoopButtonState = closedLoopButtonState == TecDefaults.EnableClosedLoopText ? TecDefaults.DisableClosedLoopText : TecDefaults.EnableClosedLoopText;
            var status = await tecModel.ControlCommand(state);
        }

        private async void ResetTec()
        {
            await tecModel.Reset();
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
                statusMessage = "Communication Error";
                return;
            }

            StatusMessage = GetErrorMessage(status[4]);
        }

        private async void SetWaveformCycles()
        {
            await tecModel.SetWaveformCyclesCommand(waveformCycles);
        }

        private async void SetSampleTime()
        {
            await tecModel.SetSampleTimeCommand(sampleTime);
        }

        private async void IncrementCounter()
        {
            Counter += 1;
            await tecModel.ControlCommand(TecDefaults.IncrementTecWaveform);
        }

        private async void Reset()
        {
            Counter = 0;

            // Reset
            await tecModel.ControlCommand(TecDefaults.ResetTecWaveform);
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
                    Thread.Sleep(50);

                    var iSenseData = await tecModel.ReadIsense();
                    ISense = Helper.GetFloatFromBigEndian(iSenseData);
                    Thread.Sleep(50);

                    var vSenseData = await tecModel.ReadVsense();
                    VSense = Helper.GetFloatFromBigEndian(vSenseData);
                    Thread.Sleep(50);

                    var status = await tecModel.ReadStatus();
                    Thread.Sleep(50);

                    ProcessStatus(status);
                    Thread.Sleep(updateDelay);
                }
                catch (Exception e)
                {
                    StatusMessage = e.Message;
                }
            }
        }

        private void InitialUpdate()
        {
            var iRefData = tecModel.ReadIref().Result;
            IRef = Helper.GetFloatFromBigEndian(iRefData);

            var iSenseData = tecModel.ReadIsense().Result;
            ISense = Helper.GetFloatFromBigEndian(iSenseData);

            var vSenseData = tecModel.ReadVsense().Result;
            VSense = Helper.GetFloatFromBigEndian(vSenseData);

            var status = tecModel.ReadStatus().Result;
            ProcessStatus(status);
        }

        private string FormatSaveProgress()
        {
            return $"{SaveProgressValue} / {ProgressMaximum}";
        }

        private string GetErrorMessage(byte value)
        {
            string response;
            TecDefaults.Errors.TryGetValue(value, out response);

            return response == null ? "Unknown" : response;
        }
    }
}
