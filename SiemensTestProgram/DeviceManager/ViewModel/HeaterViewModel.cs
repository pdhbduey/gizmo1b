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

            Tins = HeaterDefaults.Tins;


            // InitialUpdate
            selectedTin = Tins[0];
            enableState = TecDefaults.EnableText;
            closedLoopState = TecDefaults.EnableClosedLoopText;
            ProportionalGain = 0.01f;
            TRef = 0f;
            IMax = 0f;
            DerivativeGain = 0f;
            IntegralGain = 0f;
            // read tref
            // read imax

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
            ClosedLoopState = closedLoopState == TecDefaults.StartWaveformText ? TecDefaults.StopWaveformText : TecDefaults.StartWaveformText;
            
            if (string.Equals(state, TecDefaults.StartWaveformText))
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
    }
}
