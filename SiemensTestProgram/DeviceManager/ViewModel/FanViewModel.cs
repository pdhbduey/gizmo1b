// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Windows;
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
        private int delayBetweenRequests = 300;

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
            RefreshCommand = new RelayCommand(param => Update());
            // Update statuses
            InitialUpdate();
            StartUpdateTask();
        }

        public RelayCommand RefreshCommand { get; set; }

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
            var thread = new Thread(() =>
            {
                UpdateAllStatuses();
            });

            thread.SetApartmentState(ApartmentState.STA);
            thread.Start();
        }

        private void Update()
        {
            // Update
            var sensorOneValue = new byte[5];
            if (fanModel.GetFanSensorRpm(1, ref sensorOneValue))
            {
                SensorOneRpm = Helper.GetFloatFromBigEndian(sensorOneValue);
            }

            //Thread.Sleep(delayBetweenRequests);

            var sensorTwoValue = new byte[5];
            if (fanModel.GetFanSensorRpm(2, ref sensorTwoValue))
            {
                SensorTwoRpm = Helper.GetFloatFromBigEndian(sensorTwoValue);
            }

            //Thread.Sleep(delayBetweenRequests);

            var status = new byte[5];
            if (fanModel.GetFanStatus(ref status))
            {
                ProcessStatus(status);
            }
            else
            {
                StatusMessage = "Communication Error";
            }


            //Thread.Sleep(updateDelay);
        }

        /// <summary>
        /// Initial update for fan.
        /// </summary>
        private void InitialUpdate()
        {
            //var initialDelay = 10;

            // Read duty cycle/period
            var cycleOneValue = new byte[5];
            if (fanModel.GetFanPwmDutyCycle(1, ref cycleOneValue))
            {
                DutyCycleOne = Helper.GetIntFromBigEndian(cycleOneValue);
            }
            
            //Thread.Sleep(initialDelay);

            var cycleTwoValue = new byte[5];
            if (fanModel.GetFanPwmDutyCycle(2, ref cycleTwoValue))
            {
                DutyCycleTwo = Helper.GetIntFromBigEndian(cycleTwoValue);
            }
            
            //Thread.Sleep(initialDelay);

            var periodOneValue = new byte[5];
            if (fanModel.GetFanPwmPeriod(1, ref periodOneValue))
            {
                PeriodOne = Helper.GetFloatFromBigEndian(periodOneValue);
            }

            //Thread.Sleep(initialDelay);

            var periodTwoValue = new byte[5];
            if (fanModel.GetFanPwmPeriod(2, ref periodTwoValue))
            {
                PeriodTwo = Helper.GetFloatFromBigEndian(periodTwoValue);
            }
            
            //Thread.Sleep(initialDelay);

            // Sensor updates
            var sensorOneValue = new byte[5];
            if (fanModel.GetFanSensorRpm(1, ref sensorOneValue))
            {
                SensorOneRpm = Helper.GetFloatFromBigEndian(sensorOneValue);
            }

            //Thread.Sleep(initialDelay);

            var sensorTwoValue = new byte[5];
            if (fanModel.GetFanSensorRpm(2, ref sensorTwoValue))
            {
                SensorTwoRpm = Helper.GetFloatFromBigEndian(sensorTwoValue);
            }

            //Thread.Sleep(initialDelay);

            var status = new byte[5];
            if (fanModel.GetFanStatus(ref status))
            {
                ProcessStatus(status);
            }
            else
            {
                StatusMessage = "Communication Error";
            }


            //Thread.Sleep(initialDelay);
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
                    var sensorOneValue = new byte[5];
                    if (fanModel.GetFanSensorRpm(1, ref sensorOneValue))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            SensorOneRpm = Helper.GetFloatFromBigEndian(sensorOneValue);
                        }));
                        
                    }

                    Thread.Sleep(delayBetweenRequests);

                    var sensorTwoValue = new byte[5];
                    if (fanModel.GetFanSensorRpm(2, ref sensorTwoValue))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            SensorTwoRpm = Helper.GetFloatFromBigEndian(sensorTwoValue);
                        }));
                        
                    }
                    
                    Thread.Sleep(delayBetweenRequests);

                    var status = new byte[5];
                    if (fanModel.GetFanStatus(ref status))
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
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        StatusMessage = e.Message;
                    }));
                    
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
            var response = new byte[5];
            switch (channel)
            {
                case 1:
                    fanModel.SetFanPwmDutyCycle(channel, dutyCycleOne, ref response);
                    break;
                case 2:
                    fanModel.SetFanPwmDutyCycle(channel, dutyCycleTwo, ref response);
                    break;
            }
        }

        private void SetFanPeriod(int channel)
        {
            var response = new byte[5];
            switch (channel)
            {
                case 1:
                    fanModel.SetFanPwmPeriod(channel, periodTwo, ref response);
                    break;
                case 2:
                    fanModel.SetFanPwmPeriod(channel, periodTwo, ref response);
                    break;
            }
        }
    }
}
