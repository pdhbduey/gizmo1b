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
        private float periodOne;
        private float periodTwo;
        public float sensorOneRpm;
        public float sensorTwoRpm;
        private string statusMessage;

        // Update task variables
        private Task updateTask;
        private int updateDelay = 750;
        private int delayBetweenRequests = 100;

        /// <summary>
        /// Creates a new instance of the FanViewModel class.
        /// </summary>
        /// <param name="fanModel"> The fan model. </param>
        public FanViewModel(IFanModel fanModel)
        {
            this.fanModel = fanModel;
            dutyCycleOne = FanDefaults.MinimumDutyCycle;
            dutyCycleTwo = FanDefaults.MinimumDutyCycle;
            statusMessage = "";

            // Update statuses
            InitialUpdate();
            StartUpdateTask();
        }

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
                SetFanDutyCycle(channel: 1);
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
                SetFanDutyCycle(channel: 2);
            }
        }

        public float PeriodOne
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
                SetFanPeriod(channel: 1);
            }
        }

        public float PeriodTwo
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
                SetFanPeriod(channel: 2);
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
                return $"Sensor1 R.P.M: {sensorOneRpm}";
            }
        }

        public string SensorTwoRpmText
        {
            get
            {
                return $"Sensor2 R.P.M: {sensorTwoRpm}";
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
            var initialDelay = 50;

            // Read duty cycle/period
            var cycleOneValue = fanModel.GetFanPwmDutyCycle(channel: 1).Result;
            DutyCycleOne = Helper.GetIntFromBigEndian(cycleOneValue);
            Thread.Sleep(initialDelay);

            var cycleTwoValue = fanModel.GetFanPwmDutyCycle(channel: 2).Result;
            DutyCycleTwo = Helper.GetIntFromBigEndian(cycleTwoValue);
            Thread.Sleep(initialDelay);

            var periodOneValue = fanModel.GetFanPwmPeriod(channel: 1).Result;

            PeriodOne = Helper.GetFloatFromBigEndian(periodOneValue);
            Thread.Sleep(initialDelay);

            var periodTwoValue = fanModel.GetFanPwmPeriod(channel: 2).Result;
            PeriodTwo = Helper.GetFloatFromBigEndian(periodTwoValue);
            Thread.Sleep(initialDelay);

            // Sensor updates
            var sensorOneValue = fanModel.GetFanSensorRpm(sensor: 1).Result;
            SensorOneRpm = Helper.GetFloatFromBigEndian(sensorOneValue);
            Thread.Sleep(initialDelay);

            var sensorTwoValue = fanModel.GetFanSensorRpm(sensor: 2).Result;
            SensorTwoRpm = Helper.GetFloatFromBigEndian(sensorTwoValue);
            Thread.Sleep(initialDelay);

            var status = fanModel.GetFanStatus().Result;
            ProcessStatus(status);
            Thread.Sleep(initialDelay);
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
                    Thread.Sleep(delayBetweenRequests);

                    var sensorTwoValue = await fanModel.GetFanSensorRpm(sensor: 2);
                    SensorTwoRpm = Helper.GetFloatFromBigEndian(sensorTwoValue);
                    Thread.Sleep(delayBetweenRequests);
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

        private void SetFanDutyCycle(int channel)
        {
            switch (channel)
            {
                case 1:
                    fanModel.SetFanPwmDutyCycle(channel, dutyCycleOne);
                    break;
                case 2:
                    fanModel.SetFanPwmDutyCycle(channel, dutyCycleTwo);
                    break;
            }
        }

        private void SetFanPeriod(int channel)
        {
            switch (channel)
            {
                case 1:
                    fanModel.SetFanPwmPeriod(channel, periodTwo);
                    break;
                case 2:
                    fanModel.SetFanPwmPeriod(channel, periodTwo);
                    break;
            }
        }
    }
}
