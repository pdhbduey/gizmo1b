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
    public class FanViewModel : BindableBase, IDisposable
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
        private CancellationTokenSource cts;
        private CancellationToken token;
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

            SetFanPeriodOne = new RelayCommand(x => SetFanPeriod(1));
            SetFanPeriodTwo = new RelayCommand(x => SetFanPeriod(2));
            RefreshCommand = new RelayCommand(param => InitialUpdate());

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
                // SetFanDutyCycle(channel: 1);
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
                // SetFanDutyCycle(channel: 2);
            }
        }

        public RelayCommand SetFanPeriodOne { get; set; }

        public RelayCommand SetFanPeriodTwo { get; set; }

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
            cts = new CancellationTokenSource();
            token = cts.Token;

            updateTask = Task.Factory.StartNew(() =>
            {
                UpdateAllStatuses();
            }, token);
        }

        /// <summary>
        /// Initial update for fan.
        /// </summary>
        private void InitialUpdate()
        {
            var cycleOneValue = fanModel.GetFanPwmDutyCycle(1).Result;
            if (cycleOneValue.succesfulResponse)
            {
                DutyCycleOne = Helper.GetIntFromBigEndian(cycleOneValue.response);
            }

            var cycleTwoValue = fanModel.GetFanPwmDutyCycle(2).Result;
            if (cycleTwoValue.succesfulResponse)
            {
                DutyCycleTwo = Helper.GetIntFromBigEndian(cycleTwoValue.response);
            }

            var periodOneValue = fanModel.GetFanPwmPeriod(1).Result;
            if (periodOneValue.succesfulResponse)
            {
                PeriodOne = Helper.GetFloatFromBigEndian(periodOneValue.response);
            }

            var periodTwoValue = fanModel.GetFanPwmPeriod(2).Result;
            if (periodTwoValue.succesfulResponse)
            {
                PeriodTwo = Helper.GetFloatFromBigEndian(periodTwoValue.response);
            }

            var sensorOneValue = fanModel.GetFanSensorRpm(1).Result;
            if (sensorOneValue.succesfulResponse)
            {
                SensorOneRpm = Helper.GetFloatFromBigEndian(sensorOneValue.response);
            }

            var sensorTwoValue = fanModel.GetFanSensorRpm(2).Result;
            if (sensorTwoValue.succesfulResponse)
            {
                SensorTwoRpm = Helper.GetFloatFromBigEndian(sensorTwoValue.response);
            }

            var status = fanModel.GetFanStatus().Result;
            if (status.succesfulResponse)
            {
                ProcessStatus(status.response);
            }
            else
            {
                StatusMessage = "Communication Error";
            }
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
                    if (token.IsCancellationRequested == true)
                    {
                        break;
                    }

                    // Update
                    var sensorOneValue = await fanModel.GetFanSensorRpm(1);
                    if (sensorOneValue.succesfulResponse)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            SensorOneRpm = Helper.GetFloatFromBigEndian(sensorOneValue.response);
                        }));
                        
                    }

                    Thread.Sleep(delayBetweenRequests);

                    var sensorTwoValue = await fanModel.GetFanSensorRpm(2);
                    if (sensorTwoValue.succesfulResponse)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            SensorTwoRpm = Helper.GetFloatFromBigEndian(sensorTwoValue.response);
                        }));
                        
                    }
                    
                    Thread.Sleep(delayBetweenRequests);

                    var status = await fanModel.GetFanStatus();
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

        public async void SetFanDutyCycle(int channel)
        {
            var response = new byte[5];
            switch (channel)
            {
                case 1:
                    await fanModel.SetFanPwmDutyCycle(channel, dutyCycleOne);
                    break;
                case 2:
                    await fanModel.SetFanPwmDutyCycle(channel, dutyCycleTwo);
                    break;
            }
        }

        private async void SetFanPeriod(int channel)
        {
            switch (channel)
            {
                case 1:
                    await fanModel.SetFanPwmPeriod(channel, periodTwo);
                    break;
                case 2:
                    await fanModel.SetFanPwmPeriod(channel, periodTwo);
                    break;
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
