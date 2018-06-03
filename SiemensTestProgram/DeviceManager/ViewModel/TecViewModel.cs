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
    using System.Windows;

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
        private float irefCustomValue;
        private float irefValue;
        private int sliderIrefValue;
        //private int irefGain;
        private int tecPeriod;
        private int customIndex;
        private int waveformCycles;
        private int sampleTime;

        private float vSense;
        private float iSense;
        private float iRef;
        private float proportionalGain;
        private float integralGain;
        private float derivativeGain;
        private string statusMessage;
        private string customReadStatus;
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

        private void EnableToggle()
        {
            var state = enableButtonState;
            EnableButtonState = enableButtonState == TecDefaults.EnableText ? TecDefaults.DisableText : TecDefaults.EnableText;

            var response = new byte[5];
            tecModel.ControlCommand(state, ref response);
        }

        private void UpdatePeriod()
        {
            if (tecPeriod < TecDefaults.PeriodMinimum)
            {
                TecPeriod = TecDefaults.PeriodMinimum;
            }
            else if (tecPeriod > TecDefaults.PeriodMaximum)
            {
                TecPeriod = TecDefaults.PeriodMaximum;
            }

            var response = new byte[5];
            tecModel.SetPeriodCommand(tecPeriod, ref response);
        }

        private void UpdateProportionalGain()
        {
            if (proportionalGain < TecDefaults.ProportionalGainMinimum)
            {
                ProportionalGain = TecDefaults.ProportionalGainMinimum;
            }
            else if (proportionalGain > TecDefaults.ProportionalGainMaximum)
            {
                ProportionalGain = TecDefaults.ProportionalGainMaximum;
            }

            var response = new byte[5];
            tecModel.SetProportionalGainCommand(proportionalGain, ref response);
        }

        private void UpdateIntegralGain()
        {
            if (integralGain < TecDefaults.IntegralGainMinimum)
            {
                IntegralGain = TecDefaults.IntegralGainMinimum;
            }
            else if (integralGain > TecDefaults.IntegralGainMaximum)
            {
                IntegralGain = TecDefaults.IntegralGainMaximum;
            }

            var response = new byte[5];
            tecModel.SetIntegralGainCommand(integralGain, ref response);
        }

        private void UpdateDerivativeGain()
        {
            if (derivativeGain < TecDefaults.DerivativeGainMinimum)
            {
                DerivativeGain = TecDefaults.DerivativeGainMinimum;
            }
            else if (derivativeGain > TecDefaults.DerivativeGainMaximum)
            {
                DerivativeGain = TecDefaults.DerivativeGainMaximum;
            }

            var response = new byte[5];
            tecModel.SetDerivativeGainCommand(derivativeGain, ref response);
        }

        private void UpdateIrefForWaveform()
        {
            var response = new byte[5];
            tecModel.SetWaveformIrefCommand(irefCustomValue, ref response);
        }

        private void SetIref()
        {
            if (irefValue < -15.0f)
            {
                IrefValue = -15.0f;
            }
            else if (irefValue > 15.0f)
            {
                IrefValue = 15.0f;
            }

            var response = new byte[5];
            tecModel.SetIrefCommand(irefValue, ref response);
        }

        private void UpdateWaveform()
        {
            var response = new byte[5];
            tecModel.SetWaveformCommand(selectedWaveForm, ref response);
        }

        private void CaptureToggle()
        {
            CaptureButtonState = captureButtonState == TecDefaults.StartCaptureText ? TecDefaults.StopCaptureText : TecDefaults.StartCaptureText;
        }

        private void WaveformToggle()
        {
            var state = waveformButtonState;
            WaveformButtonState = waveformButtonState == TecDefaults.StartWaveformText ? TecDefaults.StopWaveformText : TecDefaults.StartWaveformText;

            var response = new byte[5];
            tecModel.ControlCommand(state, ref response);
        }

        private void ClosedLoopToggle()
        {
            var state = closedLoopButtonState;
            ClosedLoopButtonState = closedLoopButtonState == TecDefaults.EnableClosedLoopText ? TecDefaults.DisableClosedLoopText : TecDefaults.EnableClosedLoopText;

            var response = new byte[5];
            tecModel.ControlCommand(state, ref response);
        }

        private void ResetTec()
        {
            var response = new byte[5];
            tecModel.Reset(ref response);
        }

        private void UpdateIref()
        {
            var data = new byte[5];
            if (tecModel.ReadIref(ref data))
            {
                IRef = Helper.GetFloatFromBigEndian(data);
            }
        }

        private void UpdateISense()
        {
            var data = new byte[5];
            if (tecModel.ReadIsense(ref data))
            {
                ISense = Helper.GetFloatFromBigEndian(data);
            }
        }

        private void UpdateVSense()
        {
            var data = new byte[5];
            if (tecModel.ReadVsense(ref data))
            {
                VSense = Helper.GetFloatFromBigEndian(data);
            }
            
        }

        private void UpdateStatus()
        {
            var status = new byte[5];
            if (tecModel.ReadStatus(ref status))
            {
                ProcessStatus(status);
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

        private void SetWaveformCycles()
        {
            if (waveformCycles < TecDefaults.WaveformCyclesMinimum)
            {
                WaveformCycles = TecDefaults.WaveformCyclesMinimum;
            }
            else if (waveformCycles > TecDefaults.WaveformCyclesMaximum)
            {
                WaveformCycles = TecDefaults.WaveformCyclesMaximum;
            }

            var response = new byte[5];
            tecModel.SetWaveformCyclesCommand(waveformCycles, ref response);
        }

        private void SetSampleTime()
        {
            if (sampleTime < TecDefaults.SampleTimeMinimum)
            {
                SampleTime = TecDefaults.SampleTimeMinimum;
            }
            else if (sampleTime > TecDefaults.SampleTimeMaximum)
            {
                SampleTime = TecDefaults.SampleTimeMaximum;
            }

            var response = new byte[5];
            tecModel.SetSampleTimeCommand(sampleTime, ref response);
        }

        private void IncrementCounter()
        {
            //Counter += 1;

            var response = new byte[5];
            tecModel.ControlCommand(TecDefaults.IncrementTecWaveform, ref response);
            ReadCustomWaveformIndex();
        }

        private void Reset()
        {
            //Counter = 0;

            // Reset
            var response = new byte[5];
            tecModel.ControlCommand(TecDefaults.ResetTecWaveform, ref response);
            ReadCustomWaveformIndex();
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
                try
                {
                    var irefData = new byte[5];
                    if (tecModel.ReadIref(ref irefData))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            IRef = Helper.GetFloatFromBigEndian(irefData);
                        }));

                    }
                    Thread.Sleep(updateDelay);

                    var isenseData = new byte[5];
                    if (tecModel.ReadIsense(ref isenseData))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            ISense = Helper.GetFloatFromBigEndian(isenseData);
                        }));
                        
                    }
                    Thread.Sleep(updateDelay);

                    var vsenseData = new byte[5];
                    if (tecModel.ReadVsense(ref vsenseData))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            VSense = Helper.GetFloatFromBigEndian(vsenseData);
                        }));
                        
                    }
                    Thread.Sleep(updateDelay);

                    var status = new byte[5];
                    if (tecModel.ReadStatus(ref status))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            ProcessStatus(status);
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

        private void InitialUpdate()
        {
            ReadCustomWaveformIndex();
            UpdateIref();
            UpdateISense();
            UpdateVSense();
            UpdateStatus();
        }

        private void ReadCustomWaveformIndex()
        {
            var index = new byte[5];
            var indexByteArray = tecModel.ReadWaveformIndex(ref index);

            if (indexByteArray)
            {
                CustomIndex = Helper.GetIntFromBigEndian(index);
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
    }
}
