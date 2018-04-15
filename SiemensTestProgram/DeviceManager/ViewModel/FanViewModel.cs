// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Threading;
    using System.Threading.Tasks;
    using Common;
    using Common.Bindings;
    using Model;

    /// <summary>
    /// This is the view model for the fan.
    /// </summary>
    public class FanViewModel : BindableBase
    {
        // Fan variables
        private IFanModel fanModel;
        private int dutyCycleOne;
        private int dutyCycleTwo;
        private int periodOne;
        private int periodTwo;
        public float sensorOneRpm;
        public float sensorTwoRpm;
        private string statusMessage;

        // Update task variables
        private Task updateTask;
        private int updateDelay = 1500;

        /// <summary>
        /// Creates a new instance of the FanViewModel class.
        /// </summary>
        /// <param name="fanModel"> The fan model. </param>
        public FanViewModel(IFanModel fanModel)
        {
            this.fanModel = fanModel;
            dutyCycleOne = FanDefaults.MinimumDutyCycle;
            dutyCycleTwo = FanDefaults.MinimumDutyCycle;
            periodOne = FanDefaults.DefaultPeriod;
            periodTwo = FanDefaults.DefaultPeriod;
            statusMessage = "";

            // Command bindings 

            // Update statuses
            InitialUpdate();
            StartUpdateTask();
        }

        /// <summary>
        /// Fan one command binding.
        /// </summary>
        public RelayCommand SetFanOneCommand { get; set; }

        /// <summary>
        /// Fan two command binding.
        /// </summary>
        public RelayCommand SetFanTwoCommand { get; set; }

        public int DutyCycleOne
        {
            get
            {
                return dutyCycleOne;
            }

            set
            {
                if (value < FanDefaults.MinimumDutyCycle)
                {
                    dutyCycleOne = FanDefaults.MinimumDutyCycle;
                }
                else if (value > FanDefaults.MaximumDutyCycle)
                {
                    dutyCycleOne = FanDefaults.MaximumDutyCycle;
                }
                else
                {
                    dutyCycleOne = value;
                }

                OnPropertyChanged(nameof(DutyCycleOne));
            }
        }

        public int DutyCycleTwo
        {
            get
            {
                return dutyCycleTwo;
            }

            set
            {
                if (value < FanDefaults.MinimumDutyCycle)
                {
                    dutyCycleTwo = FanDefaults.MinimumDutyCycle;
                }
                else if (value > FanDefaults.MaximumDutyCycle)
                {
                    dutyCycleTwo = FanDefaults.MaximumDutyCycle;
                }
                else
                {
                    dutyCycleTwo = value;
                }

                OnPropertyChanged(nameof(DutyCycleTwo));
            }
        }

        public int PeriodOne
        {
            get
            {
                return periodOne;
            }

            set
            {
                if (value < FanDefaults.MinimumPeriod)
                {
                    periodOne = FanDefaults.MinimumPeriod;
                }
                else if (value > FanDefaults.MaximumPeriod)
                {
                    periodOne = FanDefaults.MaximumPeriod;
                }
                else
                {
                    periodOne = value;
                }

                OnPropertyChanged(nameof(PeriodOne));
            }
        }

        public int PeriodTwo
        {
            get
            {
                return periodTwo;
            }

            set
            {
                if (value < FanDefaults.MinimumPeriod)
                {
                    periodTwo = FanDefaults.MinimumPeriod;
                }
                else if (value > FanDefaults.MaximumPeriod)
                {
                    periodTwo = FanDefaults.MaximumPeriod;
                }
                else
                {
                    periodTwo = value;
                }

                OnPropertyChanged(nameof(PeriodTwo));
            }
        }

        public string StatusMessage
        {
            get
            {
                return statusMessage;
            }
            set
            {
                statusMessage = $"Status: {value}";
                OnPropertyChanged(nameof(StatusMessage));
            }
        }

        public string SensorOneRpmText
        {
            get
            {
                return $"Sensor1 R.P.M: {sensorOneRpm.ToString("0.##")}";
            }
        }

        public string SensorTwoRpmText
        {
            get
            {
                return $"Sensor2 R.P.M: {sensorTwoRpm.ToString("0.##")}";
            }
        }

        public float SensorOneRpm
        {
            get
            {
                return sensorOneRpm;
            }
            set
            {
                sensorOneRpm = value;
                OnPropertyChanged(nameof(SensorOneRpm));
                OnPropertyChanged(nameof(SensorOneRpmText));
            }
        }

        public float SensorTwoRpm
        {
            get
            {
                return sensorTwoRpm;
            }
            set
            {
                sensorTwoRpm = value;
                OnPropertyChanged(nameof(SensorTwoRpm));
                OnPropertyChanged(nameof(SensorTwoRpmText));
            }
        }

        /// <summary>
        /// Task that for reading fan status and sensor values.
        /// </summary>
        private void StartUpdateTask()
        {
            updateTask = Task.Factory.StartNew(() =>
            {
                UpdateAllStatuses();
            });
        }

        /// <summary>
        /// Initial update for fan.
        /// </summary>
        private void InitialUpdate()
        {
            var sensorOneValue = fanModel.GetFanSensorRpm(sensor: 1).Result;
            SensorOneRpm = Helper.GetFloatFromBigEndian(sensorOneValue);

            var sensorTwoValue = fanModel.GetFanSensorRpm(sensor: 2).Result;
            SensorOneRpm = Helper.GetFloatFromBigEndian(sensorTwoValue);

            var status = fanModel.GetFanStatus().Result;
            ProcessStatus(status);
        }

        /// <summary>
        /// Updates Fan values.
        /// </summary>
        private async void UpdateAllStatuses()
        {
            while (true)
            {
                try
                {
                    // Update
                    var sensorOneValue = await fanModel.GetFanSensorRpm(sensor: 1);
                    SensorOneRpm = Helper.GetFloatFromBigEndian(sensorOneValue);

                    var sensorTwoValue = await fanModel.GetFanSensorRpm(sensor: 2);
                    SensorOneRpm = Helper.GetFloatFromBigEndian(sensorTwoValue);

                    var status = await fanModel.GetFanStatus();
                    ProcessStatus(status);

                    Thread.Sleep(updateDelay);
                }
                catch (Exception e)
                {
                    StatusMessage = e.Message;
                }
            }
        }

        /// <summary>
        /// Processes read response for fan.
        /// </summary>
        /// <param name="status"> Register response. </param>
        private void ProcessStatus(byte[] status)
        {
            if (status.Length < 4)
            {
                StatusMessage = "Communication Error";
                return;
            }

            StatusMessage = GetStatusMessage(status[4]);
        }

        /// <summary>
        /// Checks text for register response.
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        private string GetStatusMessage(byte value)
        {
            string response;
            FanDefaults.Statuses.TryGetValue(value, out response);

            return response == null ? "Unknown" : response;
        }
    }
}
