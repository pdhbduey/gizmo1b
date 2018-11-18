// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Collections.Generic;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Windows;

    using Common;
    using Common.Bindings;
    using Model;

    public class TecViewModel : BindableBase, IDisposable
    {
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
        private float irefCustomValue;
        private float irefValue;
        private int sliderIrefValue;
        //private int irefGain;
        private int tecPeriod;
        private int customIndex;
        private int waveformCycles;
        private int sampleTime;
        private float voutValue;

        private float vSense;
        private float iSense;
        private float iRef;
        private float proportionalGain;
        private float integralGain;
        private float derivativeGain;
        private string statusMessage;
        private string customReadStatus;

        private Task updateTask;
        private CancellationTokenSource cts;
        private CancellationToken token;
        private const int updateDelay = 300;

        public TecViewModel(ITecModel tecModel)
        {
            this.tecModel = tecModel;

            // Initial values
            progressMaximum = 100;
            proportionalGain = 1.0f;
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
            VoutValue = TecDefaults.VoutMaximum;
            numberOfSamples = 0;
            CustomIndex = 0;
            CustomReadStatus = "No data read";

            //SliderIrefValue = 0;

            // Set commands
            EnableCommand = new RelayCommand(param => EnableToggle());
            CaptureStartStopCommand = new RelayCommand(param => CaptureToggle());
            StartStopWaveformCommand = new RelayCommand(param => WaveformToggle());
            ClosedLoopToggleCommand = new RelayCommand(param => ClosedLoopToggle());
            ResetTecCommand = new RelayCommand(param => ResetTec());
            ResetCustomWaveformCommand = new RelayCommand(param => Reset());
            UpdateIrefCommand = new RelayCommand(param => UpdateIrefForWaveform());
            IncrementCommand = new RelayCommand(param => IncrementCounter());
            RefreshCommand = new RelayCommand(param => InitialUpdate());
            GetCustomWaveformDataCommand = new RelayCommand(param => GetCustomWaveformData());

            SetTecPeriodCommand = new RelayCommand(param => UpdatePeriod());
            SetVoutCommand = new RelayCommand(param => SetVout());
            SendIrefCommand = new RelayCommand(param => SetIref());
            SetIntegralGainCommand = new RelayCommand(param => UpdateIntegralGain());
            SetProportionalGainCommand = new RelayCommand(param => UpdateProportionalGain());
            SetDerivativeGainCommand = new RelayCommand(param => UpdateDerivativeGain());
            SetWaveformIrefCommand = new RelayCommand(param => UpdateIrefForWaveform());
            SetWaveformTimeCommand = new RelayCommand(param => SetSampleTime());
            SetWaveformCyclesCommand = new RelayCommand(param => SetWaveformCycles());

            InitialUpdate();
            StartUpdateTask();
        }

        public RelayCommand GetCustomWaveformDataCommand { get; set; }
        public RelayCommand SetTecPeriodCommand { get; set; }
        public RelayCommand SetVoutCommand { get; set; }
        public RelayCommand SendIrefCommand { get; set; }
        public RelayCommand SetIntegralGainCommand { get; set; }
        public RelayCommand SetProportionalGainCommand { get; set; }
        public RelayCommand SetDerivativeGainCommand { get; set; }
        public RelayCommand SetWaveformIrefCommand { get; set; }
        public RelayCommand SetWaveformTimeCommand { get; set; }
        public RelayCommand SetWaveformCyclesCommand { get; set; }

        public RelayCommand RefreshCommand { get; set; }

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
                OnPropertyChanged(nameof(EnableButtonIsChecked));
            }
        }

        public int CustomIndex
        {
            get
            {
                return customIndex;
            }

            set
            {
                customIndex = value;
                OnPropertyChanged(nameof(CustomIndex));
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
                OnPropertyChanged(nameof(ClosedLoopButtonIsChecked));
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
                OnPropertyChanged(nameof(WaveformButtonIsChecked));
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
                proportionalGain = value;

                OnPropertyChanged(nameof(ProportionalGain));
                //UpdateProportionalGain();
            }
        }

        public float IntegralGain
        {
            get
            {
                return integralGain;
            }
            set
            {
                integralGain = value;

                OnPropertyChanged(nameof(IntegralGain));
                //UpdateIntegralGain();
            }
        }

        public float DerivativeGain
        {
            get
            {
                return derivativeGain;
            }
            set
            {
                derivativeGain = value;

                OnPropertyChanged(nameof(DerivativeGain));
                //UpdateDerivativeGain();
            }
        }

        private string integralGainText;
        public string IntegralGainText
        {
            get
            {
                return integralGainText;
            }

            set
            {
                integralGainText = value;

                if (float.TryParse(value, out var parsed))
                {
                    IntegralGain = parsed;
                }

                OnPropertyChanged(nameof(IntegralGainText));
            }
        }

        private string derivativeGainText;
        public string DerivativeGainText
        {
            get
            {
                return derivativeGainText;
            }

            set
            {
                derivativeGainText = value;

                if (float.TryParse(value, out var parsed))
                {
                    derivativeGain = parsed;
                }

                OnPropertyChanged(nameof(DerivativeGainText));
            }
        }

        private string proportionalGainText;
        public string ProportionalGainText
        {
            get
            {
                return proportionalGainText;
            }

            set
            {
                proportionalGainText = value;

                if (float.TryParse(value, out var parsed))
                {
                    proportionalGain = parsed;
                }

                OnPropertyChanged(nameof(ProportionalGainText));
            }
        }

        //
        private string voutValueText;
        public string VoutValueText
        {
            get
            {
                return voutValueText;
            }

            set
            {
                voutValueText = value;

                if (float.TryParse(value, out var parsed))
                {
                    voutValue = parsed;
                }

                OnPropertyChanged(nameof(VoutValueText));
            }
        }

        private string irefValueText;
        public string IrefValueText
        {
            get
            {
                return irefValueText;
            }

            set
            {
                irefValueText = value;

                if (float.TryParse(value, out var parsed))
                {
                    IrefValue = parsed;
                }

                OnPropertyChanged(nameof(IrefValueText));
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
                waveformCycles = value;
                
                OnPropertyChanged(nameof(WaveformCycles));

                //SetWaveformCycles();
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
                sampleTime = value;
                
                OnPropertyChanged(nameof(SampleTime));

                //SetSampleTime();
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
                tecPeriod = value;

                OnPropertyChanged(nameof(TecPeriod));
                //UpdatePeriod();
            }
        }

        public float IrefValue
        {
            get
            {
                return irefValue;
            }
            set
            {
                irefValue = value;
                
                OnPropertyChanged(nameof(IrefValue));
                //SetIref();
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

        public float IrefCustomValue
        {
            get
            {
                return irefCustomValue;
            }

            set
            {
                irefCustomValue = value; 
                OnPropertyChanged(nameof(IrefCustomValue));
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

        public float VoutValue
        {
            get
            {
                return voutValue;
            }

            set
            {
                voutValue = value;
                OnPropertyChanged(nameof(VoutValue));
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

        public string CustomReadStatus
        {
            get
            {
                return customReadStatus;
            }
            set
            {
                customReadStatus = value;

                OnPropertyChanged(nameof(CustomReadStatus));
                //UpdateTRef();
            }
        }

        private void GetCustomWaveformData()
        {
            try
            {
                var browser = new System.Windows.Forms.OpenFileDialog();
                browser.Filter = "CSV files (*.csv)|*.csv";

                string filePath = string.Empty;

                if (browser.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    filePath = browser.FileName;
                    CustomReadStatus = $"Fetching data from: {filePath}";
                }

                var data = Helper.GetCsvData(filePath);

                CustomReadStatus = $"Writing values";
                Reset();
                for (var index = 0; index < data.sampleTimes.Count; index++)
                {
                    if (SetCustomWaveformSampleTime(data.sampleTimes[index]) && SetCustomWaveformTref(data.sampleValueFirst[index]))
                    {
                        IncrementCounter();
                    }
                    else
                    {
                        throw new Exception("Writing data to registers. Please retry.");
                    }
                }

                CustomReadStatus = "Complete";
            }
            catch (Exception e)
            {
                CustomReadStatus = $"Error: {e.Message}";
            }
        }

        private bool SetCustomWaveformSampleTime(int time)
        {
            var response = tecModel.SetSampleTimeCommand(time).Result;
            return response.succesfulResponse;
        }

        private bool SetCustomWaveformTref(float customWaveformTref)
        {
            var response = tecModel.SetCustomWaveformIRefCommand(customWaveformTref).Result;
            return response.succesfulResponse;
        }

        private async void EnableToggle()
        {
            var state = enableButtonState;
            EnableButtonState = enableButtonState == TecDefaults.EnableText ? TecDefaults.DisableText : TecDefaults.EnableText;

            await tecModel.ControlCommand(state);
        }

        private async void UpdatePeriod()
        {
            if (tecPeriod < TecDefaults.PeriodMinimum)
            {
                return;
            }
            else if (tecPeriod > TecDefaults.PeriodMaximum)
            {
                return;
            }

            await tecModel.SetPeriodCommand(tecPeriod);
        }

        private async void UpdateProportionalGain()
        {
            if (proportionalGain < TecDefaults.ProportionalGainMinimum)
            {
                return;
            }
            else if (proportionalGain > TecDefaults.ProportionalGainMaximum)
            {
                return;
            }

            await tecModel.SetProportionalGainCommand(proportionalGain);
        }

        private async void UpdateIntegralGain()
        {
            if (integralGain < TecDefaults.IntegralGainMinimum)
            {
                return;
            }
            else if (integralGain > TecDefaults.IntegralGainMaximum)
            {
                return;
            }

            await tecModel.SetIntegralGainCommand(integralGain);
        }

        private async void UpdateDerivativeGain()
        {
            if (derivativeGain < TecDefaults.DerivativeGainMinimum)
            {
                return;
            }
            else if (derivativeGain > TecDefaults.DerivativeGainMaximum)
            {
                return;
            }

            await tecModel.SetDerivativeGainCommand(derivativeGain);
        }

        private async void UpdateIrefForWaveform()
        {
            await tecModel.SetWaveformIrefCommand(irefCustomValue);
        }

        private async void SetIref()
        {
            if (irefValue < -15.0f)
            {
                IrefValue = -15.0f;
            }
            else if (irefValue > 15.0f)
            {
                IrefValue = 15.0f;
            }

            await tecModel.SetIrefCommand(irefValue);
        }

        private async void SetVout()
        {
            if (voutValue < TecDefaults.VoutMinimum)
            {
                return;
            }
            else if (voutValue > TecDefaults.VoutMaximum)
            {
                return;
            }

            await tecModel.SetVoutCommand(voutValue);
        }

        private async void UpdateWaveform()
        {
            await tecModel.SetWaveformCommand(selectedWaveForm);
        }

        private void CaptureToggle()
        {
            CaptureButtonState = captureButtonState == TecDefaults.StartCaptureText ? TecDefaults.StopCaptureText : TecDefaults.StartCaptureText;
        }

        private async void WaveformToggle()
        {
            var state = waveformButtonState;
            WaveformButtonState = waveformButtonState == TecDefaults.StartWaveformText ? TecDefaults.StopWaveformText : TecDefaults.StartWaveformText;

            await tecModel.ControlCommand(state);
        }

        private async void ClosedLoopToggle()
        {
            var state = closedLoopButtonState;
            ClosedLoopButtonState = closedLoopButtonState == TecDefaults.EnableClosedLoopText ? TecDefaults.DisableClosedLoopText : TecDefaults.EnableClosedLoopText;

            await tecModel.ControlCommand(state);
        }

        private async void ResetTec()
        {
            await tecModel.Reset();
        }

        private async void UpdateIref()
        {
            var data = await tecModel.ReadIref();
            if (data.succesfulResponse)
            {
                IRef = Helper.GetFloatFromBigEndian(data.response);
            }
        }

        private async void UpdateISense()
        {
            var data = await tecModel.ReadIsense();
            if (data.succesfulResponse)
            {
                ISense = Helper.GetFloatFromBigEndian(data.response);
            }
        }

        private async void UpdateVSense()
        {
            var data = await tecModel.ReadVsense();
            if (data.succesfulResponse)
            {
                VSense = Helper.GetFloatFromBigEndian(data.response);
            }
            
        }

        private async void UpdateStatus()
        {
            var status = await tecModel.ReadStatus();
            if (status.succesfulResponse)
            {
                ProcessStatus(status.response);
            }
            else
            {
                StatusMessage = "Communication Error";
            }
        }

        private void ProcessStatus(byte[] status)
        {
            if (status.Length < 4)
            {
                StatusMessage = "Communication Error";
                return;
            }

            StatusMessage = GetErrorMessage(status[4]);
        }

        private async void SetWaveformCycles()
        {
            if (waveformCycles < TecDefaults.WaveformCyclesMinimum)
            {
                return;
            }
            else if (waveformCycles > TecDefaults.WaveformCyclesMaximum)
            {
                return;
            }

            await tecModel.SetWaveformCyclesCommand(waveformCycles);
        }

        private async void SetSampleTime()
        {
            if (sampleTime < TecDefaults.SampleTimeMinimum)
            {
                return;
            }
            else if (sampleTime > TecDefaults.SampleTimeMaximum)
            {
                return;
            }

            await tecModel.SetSampleTimeCommand(sampleTime);
        }

        private async void IncrementCounter()
        {
            await tecModel.ControlCommand(TecDefaults.IncrementTecWaveform);
            ReadCustomWaveformIndex();
        }

        private async void Reset()
        {
            await tecModel.ControlCommand(TecDefaults.ResetTecWaveform);
            ReadCustomWaveformIndex();
        }

        private void StartUpdateTask()
        {
            cts = new CancellationTokenSource();
            token = cts.Token;

            updateTask = Task.Factory.StartNew(() =>
            {
                UpdateAllStatuses();
            }, token);
        }

        private async void UpdateAllStatuses()
        {
            while (true)
            {
                try
                {
                    if (token.IsCancellationRequested)
                    {
                        break;
                    }

                    var irefData = await tecModel.ReadIref();
                    if (irefData.succesfulResponse)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            IRef = Helper.GetFloatFromBigEndian(irefData.response);
                        }));

                    }
                    Thread.Sleep(updateDelay);

                    var isenseData = await tecModel.ReadIsense();
                    if (isenseData.succesfulResponse)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            ISense = Helper.GetFloatFromBigEndian(isenseData.response);
                        }));
                        
                    }
                    Thread.Sleep(updateDelay);

                    var vsenseData = await tecModel.ReadVsense();
                    if (vsenseData.succesfulResponse)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            VSense = Helper.GetFloatFromBigEndian(vsenseData.response);
                        }));
                        
                    }
                    Thread.Sleep(updateDelay);

                    var status = await tecModel.ReadStatus();
                    if (status.succesfulResponse)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            ProcessStatus(status.response);
                        }));
                        
                    }
                    else
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            StatusMessage = "Communication Error";
                        }));
                        
                    }
                    Thread.Sleep(updateDelay);
                }
                catch (Exception e)
                {
                    StatusMessage = e.Message;
                }
            }
        }

        public bool ClosedLoopButtonIsChecked
        {
            get
            {
                if (closedLoopButtonState.Equals(TecDefaults.EnableClosedLoopText))
                {
                    return false;
                }

                return true;
            }
        }
        public bool EnableButtonIsChecked
        {
            get
            {
                if (enableButtonState.Equals(TecDefaults.EnableText))
                {
                    return false;
                }

                return true;
            }
        }

        public bool WaveformButtonIsChecked
        {
            get
            {
                if (waveformButtonState.Equals(TecDefaults.StartWaveformText))
                {
                    return false;
                }

                return true;
            }
        }


        private void InitialUpdate()
        {
            ReadCustomWaveformIndex();
            UpdateIref();
            UpdateISense();
            UpdateVSense();
            UpdateStatus();

            var vmaxResponse = tecModel.ReadVmax().Result;
            if (vmaxResponse.succesfulResponse)
            {
                VoutValueText = Helper.GetFloatFromBigEndian(vmaxResponse.response).ToString("0.##");
            }

            var irefSamplesResponse = tecModel.ReadIrefSamples().Result;
            if (irefSamplesResponse.succesfulResponse)
            {
                IrefValueText = Helper.GetFloatFromBigEndian(irefSamplesResponse.response).ToString("0.##");
            }

            var readCyclesResponse = tecModel.ReadCycles().Result;
            if (readCyclesResponse.succesfulResponse)
            {
                WaveformCycles = Helper.GetIntFromBigEndian(readCyclesResponse.response);
            }

            var readPeriodResponse = tecModel.ReadPeriod().Result;
            if (readPeriodResponse.succesfulResponse)
            {
                TecPeriod = Helper.GetIntFromBigEndian(readPeriodResponse.response);
            }

            var integralGainResponse = tecModel.ReadIntegralGain().Result;
            if (integralGainResponse.succesfulResponse)
            {
                IntegralGainText = Helper.GetFloatFromBigEndian(integralGainResponse.response).ToString("0.##");
            }

            var derivativeGainResponse = tecModel.ReadDerivativeGain().Result;
            if (derivativeGainResponse.succesfulResponse)
            {
                DerivativeGainText = Helper.GetFloatFromBigEndian(derivativeGainResponse.response).ToString("0.##");
            }

            var proportionalGainResponse = tecModel.ReadProportionalGain().Result;
            if (proportionalGainResponse.succesfulResponse)
            {
                ProportionalGainText = Helper.GetFloatFromBigEndian(proportionalGainResponse.response).ToString("0.##");
            }

            var controlStateResponse = tecModel.ReadControlState().Result;
            if (controlStateResponse.succesfulResponse)
            {
                var buttonBytes = controlStateResponse.response[4];
                EnableButtonState = Helper.IsBitSet(buttonBytes, 1) ? TecDefaults.DisableText : TecDefaults.EnableText;
                WaveformButtonState = Helper.IsBitSet(buttonBytes, 2) ? TecDefaults.StopWaveformText : TecDefaults.StartWaveformText;
                ClosedLoopButtonState = Helper.IsBitSet(buttonBytes, 5) ? TecDefaults.DisableClosedLoopText : TecDefaults.EnableClosedLoopText;
            }

            var readWaveformResponse = tecModel.ReadWaveform().Result;
            if (readWaveformResponse.succesfulResponse)
            {
                var waveformPosition = (int)readWaveformResponse.response[4];
                if (waveformPosition < 0 || waveformPosition > 4)
                {
                    waveformPosition = 0;
                }

                SelectedWaveForm = Waveforms[waveformPosition];
            }
        }

        private async void ReadCustomWaveformIndex()
        {
            var index = await tecModel.ReadWaveformIndex();

            if (index.succesfulResponse)
            {
                CustomIndex = Helper.GetIntFromBigEndian(index.response);
            }
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

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects).
                    cts?.Cancel();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                updateTask = null;

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~DacViewModel() {
        //   // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
        //   Dispose(false);
        // }

        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
            // TODO: uncomment the following line if the finalizer is overridden above.
            // GC.SuppressFinalize(this);
        }
        #endregion
    }
}
