using Common;
using Common.Bindings;
using DeviceManager.Model;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace DeviceManager.ViewModel
{
    public class HeaterViewModel : BindableBase
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

            InitialUpdate();
            StartUpdateTask();
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
                return $"Temperature One: {temperatureOne.ToString("0.##")} °C";
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
                return $"Temperature Two: {temperatureTwo.ToString("0.##")} °C";
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
                return $"Temperature Three: {temperatureThree.ToString("0.##")} °C";
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
                return $"Temperature Four: {temperatureFour.ToString("0.##")} °C";
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
            var thread = new Thread(() =>
            {
                UpdateAllStatuses();
            });

            thread.SetApartmentState(ApartmentState.STA);
            thread.Start();
        }

        private void InitialUpdate()
        {
            var vData = heaterModel.ReadVSenseCommand().Result;
            if (vData.succesfulResponse)
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    VSense = Helper.GetFloatFromBigEndian(vData.response);
                }));

            }

            var isenseData = heaterModel.ReadISenseCommand().Result;
            if (isenseData.succesfulResponse)
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    ISense = Helper.GetFloatFromBigEndian(isenseData.response);
                }));

            }

            var TemperatureOneData = heaterModel.ReadTemperatureOne().Result;
            if (TemperatureOneData.succesfulResponse)
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    TemperatureOne = Helper.GetFloatFromBigEndian(TemperatureOneData.response);
                }));

            }

            var TemperatureTwoData = heaterModel.ReadTemperatureTwo().Result;
            if (TemperatureTwoData.succesfulResponse)
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    TemperatureTwo = Helper.GetFloatFromBigEndian(TemperatureTwoData.response);
                }));

            }

            var TemperatureThreeData = heaterModel.ReadTemperatureThree().Result;
            if (TemperatureThreeData.succesfulResponse)
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    TemperatureThree = Helper.GetFloatFromBigEndian(TemperatureThreeData.response);
                }));

            }

            var TemperatureFourData = heaterModel.ReadTemperatureFour().Result;
            if (TemperatureFourData.succesfulResponse)
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    TemperatureFour = Helper.GetFloatFromBigEndian(TemperatureFourData.response);
                }));
            }

            var status = heaterModel.ReadStatusCommand().Result;
            if (status.succesfulResponse)
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    ProcessStatus(status.response);
                }));
            }
            else
            {
                Application.Current.Dispatcher.Invoke(new Action(() =>
                {
                    StatusMessage = "Communication Error";
                }));

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
        if (HeaterDefaults.StatusValues.TryGetValue(status[3], out value))
        {
            StatusMessage = value;
        }
    }

        private async void EnableToggle()
        {
            var state = enableState;
            EnableState = enableState == TecDefaults.EnableText ? TecDefaults.DisableText : TecDefaults.EnableText;

            if (string.Equals(state, TecDefaults.EnableText))
            {
                await heaterModel.EnableCommand();
            }
            else
            {
                await heaterModel.DisableCommand();
            }
        }

        private async void ClosedLoopToggle()
        {
            var state = closedLoopState;
            ClosedLoopState = closedLoopState == TecDefaults.EnableClosedLoopText ? TecDefaults.DisableClosedLoopText : TecDefaults.EnableClosedLoopText;
            
            if (string.Equals(state, TecDefaults.EnableClosedLoopText))
            {
                await heaterModel.StartClosedLoopCommand();
            }
            else
            {
                await heaterModel.StopClosedLoopCommand();
            }
        }

        private async void UpdateProportionalGain()
        {
            if (proportionalGain < HeaterDefaults.ProportionalGainMinimum)
            {
                ProportionalGain = HeaterDefaults.ProportionalGainMinimum;
            }
            else if (proportionalGain > HeaterDefaults.ProportionalGainMaximum)
            {
                ProportionalGain = HeaterDefaults.ProportionalGainMaximum;
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
                IntegralGain = HeaterDefaults.IntegralGainMinimum;
            }
            else if (integralGain > HeaterDefaults.IntegralGainMaximum)
            {
                IntegralGain = HeaterDefaults.IntegralGainMaximum;
            }

            await heaterModel.SetIntegralGainCommand(integralGain);
        }

        private async void UpdateDerivativeGain()
        {
            if (derivativeGain < HeaterDefaults.DerivativeGainMinimum)
            {
                DerivativeGain = HeaterDefaults.DerivativeGainMinimum;
            }
            else if (derivativeGain > HeaterDefaults.DerivativeGainMaximum)
            {
                DerivativeGain = HeaterDefaults.DerivativeGainMaximum;
            }

            await heaterModel.SetDerivativeGainCommand(derivativeGain);
        }

        private async void UpdateTRef()
        {
            if (tRef < HeaterDefaults.TRefMinimum)
            {
                TRef = HeaterDefaults.TRefMinimum;
            }
            else if (tRef > HeaterDefaults.TRefMaximum)
            {
                TRef = HeaterDefaults.TRefMaximum;
            }

            await heaterModel.SetTRefCommand(tRef);
        }

        private async void UpdateIMax()
        {
            if (iMax < HeaterDefaults.IMaxMinimum)
            {
                IMax = HeaterDefaults.IMaxMinimum;
            }
            else if (iMax > HeaterDefaults.IMaxMaximum)
            {
                IMax = HeaterDefaults.IMaxMaximum;
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
    }
}
