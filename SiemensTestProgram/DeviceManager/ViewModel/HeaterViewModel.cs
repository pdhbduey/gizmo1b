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
    using DeviceManager.Model;

    public class HeaterViewModel : BindableBase, IDisposable
    {
        IHeaterModel heaterModel;

        private float proportionalGain;
        private float integralGain;
        private float derivativeGain;
        private float tRef;
        private float iMax;
        private float vSense;
        private float iSense;
        private float temperatureOne;
        private float temperatureTwo;
        private float temperatureThree;
        private float temperatureFour;
        private string enableState;
        private string closedLoopState;
        private string statusMessage;
        private string selectedTin;
        private string customReadStatus;
        private int customIndex;
        private int waveformCycles;
        private string waveformButtonState;

        private Task updateTask;
        private CancellationTokenSource cts;
        private CancellationToken token;
        private const int updateDelay = 300;

        public HeaterViewModel(IHeaterModel heaterModel)
        {
            this.heaterModel = heaterModel;

            SetIntegralGainCommand = new RelayCommand(param => UpdateIntegralGain());
            SetProportionalGainCommand = new RelayCommand(param => UpdateProportionalGain());
            SetDerivativeGainCommand = new RelayCommand(param => UpdateDerivativeGain());
            SetTRefCommand = new RelayCommand(param => UpdateTRef());
            SetIMaxCommand = new RelayCommand(param => UpdateIMax());
            EnableCommand = new RelayCommand(param => EnableToggle());
            StartClosedLoopCommand = new RelayCommand(param => ClosedLoopToggle());
            GetCustomWaveformDataCommand = new RelayCommand(param => GetCustomWaveformData());
            ResetCustomWaveformCommand = new RelayCommand(param => Reset());
            SetWaveformCyclesCommand = new RelayCommand(param => SetWaveformCycles());
            StartStopWaveformCommand = new RelayCommand(param => WaveformToggle());

            Tins = HeaterDefaults.Tins;
            selectedTin = Tins[0];
            enableState = TecDefaults.EnableText;
            closedLoopState = TecDefaults.EnableClosedLoopText;
            ProportionalGain = 1.00f;
            TRef = 0f;
            IMax = 0f;
            DerivativeGain = 0f;
            IntegralGain = 0f;
            CustomIndex = 0;
            CustomReadStatus = "No data read";
            WaveformCycles = TecDefaults.WaveformCyclesMinimum;
            waveformButtonState = TecDefaults.StartWaveformText;

            InitialUpdate();
            StartUpdateTask();
        }

        public RelayCommand StartStopWaveformCommand { get; set; }

        private async void WaveformToggle()
        {
            var state = waveformButtonState;
            
            var response = await heaterModel.ControlCommand(state);
            if (response.succesfulResponse)
            {
                WaveformButtonState = waveformButtonState == TecDefaults.StartWaveformText ? TecDefaults.StopWaveformText : TecDefaults.StartWaveformText;
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
                OnPropertyChanged(nameof(IsWaveformChecked));
            }
        }

        public RelayCommand SetWaveformCyclesCommand { get; set; }

        private async void SetWaveformCycles()
        {
            if (waveformCycles < HeaterDefaults.WaveformCyclesMinimum)
            {
                WaveformCycles = HeaterDefaults.WaveformCyclesMinimum;
            }
            else if (waveformCycles > HeaterDefaults.WaveformCyclesMaximum)
            {
                WaveformCycles = HeaterDefaults.WaveformCyclesMaximum;
            }

            await heaterModel.SetWaveformCyclesCommand(waveformCycles);
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

        public List<string> Tins { get; set; }

        public string SelectedTin
        {
            get
            {
                return selectedTin;
            }

            set
            {
                selectedTin = value;
                OnPropertyChanged(nameof(SelectedTin));
                UpdateTin();
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

        private string imaxText;
        public string ImaxText
        {
            get
            {
                return imaxText;
            }

            set
            {
                imaxText = value;

                if (float.TryParse(value, out var parsed))
                {
                    IMax = parsed;
                }

                OnPropertyChanged(nameof(ImaxText));
            }
        }

        public float IMax
        {
            get
            {
                return iMax;
            }
            set
            {
                iMax = value;

                OnPropertyChanged(nameof(IMax));
                //UpdateIMax();
            }
        }

        private string trefText;
        public string TrefText
        {
            get
            {
                return trefText;
            }

            set
            {
                trefText = value;

                if (float.TryParse(value, out var parsed))
                {
                    TRef = parsed;
                }

                OnPropertyChanged(nameof(TrefText));
            }
        }

        public float TRef
        {
            get
            {
                return tRef;
            }
            set
            {
                tRef = value;

                OnPropertyChanged(nameof(TRef));
                //UpdateTRef();
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

        /// <summary>
        /// Text value of Temperature One.
        /// </summary>
        public string TemperatureOneText
        {
            get
            {
                return $"Temperature 1: {temperatureOne.ToString("0.##")} °C";
            }
        }

        /// <summary>
        /// Value for Temperature One.
        /// </summary>
        public float TemperatureOne
        {
            get
            {
                return temperatureOne;
            }

            set
            {
                temperatureOne = value;
                OnPropertyChanged(nameof(TemperatureOneText));
            }
        }

        /// <summary>
        /// Text value of Temperature One.
        /// </summary>
        public string TemperatureTwoText
        {
            get
            {
                return $"Temperature 2: {temperatureTwo.ToString("0.##")} °C";
            }
        }

        /// <summary>
        /// Value for Temperature One.
        /// </summary>
        public float TemperatureTwo
        {
            get
            {
                return temperatureTwo;
            }

            set
            {
                temperatureTwo = value;
                OnPropertyChanged(nameof(TemperatureTwoText));
            }
        }

        /// <summary>
        /// Text value of Temperature One.
        /// </summary>
        public string TemperatureThreeText
        {
            get
            {
                return $"Temperature 3: {temperatureThree.ToString("0.##")} °C";
            }
        }

        /// <summary>
        /// Value for Temperature One.
        /// </summary>
        public float TemperatureThree
        {
            get
            {
                return temperatureThree;
            }

            set
            {
                temperatureThree = value;
                OnPropertyChanged(nameof(TemperatureThreeText));
            }
        }

        /// <summary>
        /// Text value of Temperature One.
        /// </summary>
        public string TemperatureFourText
        {
            get
            {
                return $"Temperature 4: {temperatureFour.ToString("0.##")} °C";
            }
        }

        /// <summary>
        /// Value for Temperature One.
        /// </summary>
        public float TemperatureFour
        {
            get
            {
                return temperatureFour;
            }

            set
            {
                temperatureFour = value;
                OnPropertyChanged(nameof(TemperatureFourText));
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

        public string ClosedLoopState
        {
            get
            {
                return closedLoopState;
            }

            set
            {
                closedLoopState = value;
                OnPropertyChanged(nameof(ClosedLoopState));
                OnPropertyChanged(nameof(IsClosedLoopChecked));
            }
        }

        public string EnableState
        {
            get
            {
                return enableState;
            }

            set
            {
                enableState = value;
                OnPropertyChanged(nameof(EnableState));
                OnPropertyChanged(nameof(IsEnabledChecked));
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

        public RelayCommand GetCustomWaveformDataCommand { get; set; }

        public RelayCommand ResetCustomWaveformCommand { get; set; }

        public RelayCommand StartClosedLoopCommand { get; set; }

        public RelayCommand EnableCommand { get; set; }

        public RelayCommand SetIntegralGainCommand { get; set; }

        public RelayCommand SetProportionalGainCommand { get; set; }

        public RelayCommand SetDerivativeGainCommand { get; set; }

        public RelayCommand SetTRefCommand { get; set; }

        public RelayCommand SetIMaxCommand { get; set; }

        private void StartUpdateTask()
        {
            cts = new CancellationTokenSource();
            token = cts.Token;

            updateTask = Task.Factory.StartNew(() =>
            {
                UpdateAllStatuses();
            }, token);
        }

        public bool IsEnabledChecked
        {
            get => enableState == TecDefaults.DisableText;
        }

        public bool IsClosedLoopChecked
        {
            get => closedLoopState == TecDefaults.DisableClosedLoopText;
        }

        public bool IsWaveformChecked
        {
            get => waveformButtonState == TecDefaults.StopWaveformText;
        }

        private void InitialUpdate()
        {
            var integralData = heaterModel.ReadIntegralGain().Result;
            if (integralData.succesfulResponse)
            {
                IntegralGainText = Helper.GetFloatFromBigEndian(integralData.response).ToString("0.##");
            }

            var derivativeData = heaterModel.ReadDerivativeGain().Result;
            if (derivativeData.succesfulResponse)
            {
                DerivativeGainText = Helper.GetFloatFromBigEndian(derivativeData.response).ToString("0.##");
            }

            var proportionalData = heaterModel.ReadProportionalGain().Result;
            if (proportionalData.succesfulResponse)
            {
                ProportionalGainText = Helper.GetFloatFromBigEndian(proportionalData.response).ToString("0.##");
            }

            var imaxData = heaterModel.ReadImax().Result;
            if (imaxData.succesfulResponse)
            {
                ImaxText = Helper.GetFloatFromBigEndian(imaxData.response).ToString("0.##");
            }

            var trefData = heaterModel.ReadTref().Result;
            if (trefData.succesfulResponse)
            {
                TrefText = Helper.GetFloatFromBigEndian(trefData.response).ToString("0.##");
            }

            var cyclesData = heaterModel.ReadCycles().Result;
            if (cyclesData.succesfulResponse)
            {
                WaveformCycles = Helper.GetIntFromBigEndian(cyclesData.response);
            }

            var selectData = heaterModel.ReadSelect().Result;
            if (selectData.succesfulResponse)
            {
                if (HeaterDefaults.TinReadMapping.TryGetValue(selectData.response[4], out var selectedDataTin))
                {
                    SelectedTin = selectedDataTin;
                }
            }

            var controlData = heaterModel.ReadControl().Result;
            if (controlData.succesfulResponse)
            {
                if (Helper.IsBitSet(controlData.response[4], 1))
                {
                    EnableState = TecDefaults.DisableText;
                }
                else
                {
                    EnableState = TecDefaults.EnableText;
                }

                if (Helper.IsBitSet(controlData.response[4], 2))
                {
                    WaveformButtonState = TecDefaults.StopWaveformText;
                }
                else
                {
                    WaveformButtonState = TecDefaults.StartWaveformText;
                }

                if (Helper.IsBitSet(controlData.response[4], 5))
                {
                    ClosedLoopState = TecDefaults.DisableClosedLoopText;
                }
                else
                {
                    // stop closed loop
                    ClosedLoopState = TecDefaults.EnableClosedLoopText;
                }
            }

            var vData = heaterModel.ReadVSenseCommand().Result;
            if (vData.succesfulResponse)
            {
                VSense = Helper.GetFloatFromBigEndian(vData.response);
            }

            var isenseData = heaterModel.ReadISenseCommand().Result;
            if (isenseData.succesfulResponse)
            {
                ISense = Helper.GetFloatFromBigEndian(isenseData.response);
            }

            var TemperatureOneData = heaterModel.ReadTemperatureOne().Result;
            if (TemperatureOneData.succesfulResponse)
            {
                TemperatureOne = Helper.GetFloatFromBigEndian(TemperatureOneData.response);
            }

            var TemperatureTwoData = heaterModel.ReadTemperatureTwo().Result;
            if (TemperatureTwoData.succesfulResponse)
            {
                TemperatureTwo = Helper.GetFloatFromBigEndian(TemperatureTwoData.response);
            }

            var TemperatureThreeData = heaterModel.ReadTemperatureThree().Result;
            if (TemperatureThreeData.succesfulResponse)
            {
                 TemperatureThree = Helper.GetFloatFromBigEndian(TemperatureThreeData.response);
            }

            var TemperatureFourData = heaterModel.ReadTemperatureFour().Result;
            if (TemperatureFourData.succesfulResponse)
            {
                TemperatureFour = Helper.GetFloatFromBigEndian(TemperatureFourData.response);
            }

            var status = heaterModel.ReadStatusCommand().Result;
            if (status.succesfulResponse)
            {
                 ProcessStatus(status.response);
            }
            else
            {
                StatusMessage = "Communication Error";
            }
        }

        private async void UpdateAllStatuses()
        {
            while (true)
            {
                var vData = await heaterModel.ReadVSenseCommand();
                if (vData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        VSense = Helper.GetFloatFromBigEndian(vData.response);
                    }));
                    
                }

                Thread.Sleep(updateDelay);

                var isenseData = await heaterModel.ReadISenseCommand();
                if (isenseData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        ISense = Helper.GetFloatFromBigEndian(isenseData.response);
                    }));

                }
                Thread.Sleep(updateDelay);

                var TemperatureOneData = await heaterModel.ReadTemperatureOne();
                if (TemperatureOneData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        TemperatureOne = Helper.GetFloatFromBigEndian(TemperatureOneData.response);
                    }));

                }

                Thread.Sleep(updateDelay);

                var TemperatureTwoData = await heaterModel.ReadTemperatureTwo();
                if (TemperatureTwoData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        TemperatureTwo = Helper.GetFloatFromBigEndian(TemperatureTwoData.response);
                    }));

                }

                Thread.Sleep(updateDelay);

                var TemperatureThreeData = await heaterModel.ReadTemperatureThree();
                if (TemperatureThreeData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        TemperatureThree = Helper.GetFloatFromBigEndian(TemperatureThreeData.response);
                    }));

                }

                Thread.Sleep(updateDelay);

                var TemperatureFourData = await heaterModel.ReadTemperatureFour();
                if (TemperatureFourData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        TemperatureFour = Helper.GetFloatFromBigEndian(TemperatureFourData.response);
                    }));
                }

                Thread.Sleep(updateDelay);

                var status = await heaterModel.ReadStatusCommand();
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
        }

    private void ProcessStatus(byte[] status)
    {
        if (status.Length < 4)
        {
            StatusMessage = "Communication Error";
            return;
        }

        string value;
        if (HeaterDefaults.StatusValues.TryGetValue(status[4], out value))
        {
            StatusMessage = value;
        }
    }

        private async void EnableToggle()
        {
            var state = enableState;
            

            if (string.Equals(state, TecDefaults.EnableText))
            {
                var setEnabled = await heaterModel.EnableCommand();
                if (setEnabled.succesfulResponse)
                {
                    EnableState = TecDefaults.DisableText;
                }
            }
            else
            {
                var setEnabled = await heaterModel.DisableCommand();
                if (setEnabled.succesfulResponse)
                {
                    EnableState = TecDefaults.EnableText;
                }
            }
        }

        private async void ClosedLoopToggle()
        {
            var state = closedLoopState;
            
            
            if (string.Equals(state, TecDefaults.EnableClosedLoopText))
            {
                var response = await heaterModel.StartClosedLoopCommand();
                if (response.succesfulResponse)
                {
                    ClosedLoopState = TecDefaults.DisableClosedLoopText;
                }
            }
            else
            {
                var response = await heaterModel.StopClosedLoopCommand();
                if (response.succesfulResponse)
                {
                    ClosedLoopState = TecDefaults.EnableClosedLoopText;
                }
            }
        }

        private async void UpdateProportionalGain()
        {
            if (proportionalGain < HeaterDefaults.ProportionalGainMinimum)
            {
                return;
            }
            else if (proportionalGain > HeaterDefaults.ProportionalGainMaximum)
            {
                return;
            }

            await heaterModel.SetProportionalGainCommand(proportionalGain);
        }

        private async void UpdateTin()
        {
            await heaterModel.SetTinCommand(selectedTin);
        }

        private async void UpdateIntegralGain()
        {
            if (integralGain < HeaterDefaults.IntegralGainMinimum)
            {
                return;
            }
            else if (integralGain > HeaterDefaults.IntegralGainMaximum)
            {
                return;
            }

            await heaterModel.SetIntegralGainCommand(integralGain);
        }

        private async void UpdateDerivativeGain()
        {
            if (derivativeGain < HeaterDefaults.DerivativeGainMinimum)
            {
                return;
            }
            else if (derivativeGain > HeaterDefaults.DerivativeGainMaximum)
            {
                return;
            }

            await heaterModel.SetDerivativeGainCommand(derivativeGain);
        }

        private async void UpdateTRef()
        {
            if (tRef < HeaterDefaults.TRefMinimum)
            {
                return;
            }
            else if (tRef > HeaterDefaults.TRefMaximum)
            {
                return;
            }

            await heaterModel.SetTRefCommand(tRef);
        }

        private async void UpdateIMax()
        {
            if (iMax < HeaterDefaults.IMaxMinimum)
            {
                return;
            }
            else if (iMax > HeaterDefaults.IMaxMaximum)
            {
                return;
            }

            await heaterModel.SetIMaxCommand(iMax);
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
            catch(Exception e)
            {
                CustomReadStatus = $"Error: {e.Message}";
            }
        }

        private bool SetCustomWaveformSampleTime(int time)
        {
            var response = heaterModel.SetCustomWaveformTimeCommand(time).Result;
            return response.succesfulResponse;
        }

        private bool SetCustomWaveformTref(float customWaveformTref)
        {
            var response = heaterModel.SetCustomWaveformTRefCommand(customWaveformTref).Result;
            return response.succesfulResponse;
        }

        private async void IncrementCounter()
        {
            var incrementResponse = await heaterModel.IncrementCounterCommand();

            if (incrementResponse.succesfulResponse)
            {
                ReadCustomWaveformIndex();
            }
        }

        private void Reset()
        {
            var resetResponse = heaterModel.ResetCounterCommand().Result;
            if (resetResponse.succesfulResponse)
            {
                ReadCustomWaveformIndex();
            }
        }

        private async void ReadCustomWaveformIndex()
        {
            var customIndexData = await heaterModel.ReadWaveformIndex();
            if (customIndexData.succesfulResponse)
            {
                CustomIndex = Helper.GetIntFromBigEndian(customIndexData.response);
            }
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

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion
    }
}
