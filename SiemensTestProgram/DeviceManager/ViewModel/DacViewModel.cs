// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Windows;

    using Common;
    using Common.Bindings;
    using DeviceManager.Model;

    /// <summary>
    /// This class is responsible for updating DAC.
    /// </summary>
    public class DacViewModel : BindableBase, IDisposable
    {
        private IDacModel dacModel;
        private float voltageValue;
        private int sliderVoltageValue;
        private string dacStatus;

        private Task updateTask;
        CancellationTokenSource cts;
        CancellationToken token;
        private int updateDelay = 1000;

        public DacViewModel(IDacModel dacModel)
        {
            // Set default settings
            this.dacModel = dacModel;
            SendDacValueCommand = new RelayCommand(param => SendDacValue());
            dacStatus = string.Empty;

            // Check DAC status
            Update();

            StartUpdateTask();
            
            RefreshCommand = new RelayCommand(param => Update());
        }

        /// <summary>
        /// Command to set the DAC.
        /// </summary>
        public RelayCommand SendDacValueCommand { get; set; }

        public RelayCommand RefreshCommand { get; set; }

        private void Update()
        {
            var dacValue = dacModel.GetDacValueCommand().Result;

            if (dacValue.succesfulResponse)
            {
                VoltageValue = Helper.GetFloatFromBigEndian(dacValue.response);
                VoltageValueString = string.Format(voltageValue.ToString("0.##"));
            }
            else
            {
                SliderVoltageValue = 250;
            }

            var status = dacModel.ReadDacStatusCommand().Result;

            if (status.succesfulResponse)
            {
                ProcessStatus(status.response);
            }
            else
            {
                DacStatus = "Communication Error";
            }
        }

        /// <summary>
        /// The DAC status.
        /// </summary>
        public string DacStatus
        {
            get
            {
                return dacStatus;
            }

            set
            {
                dacStatus = value;
                OnPropertyChanged(nameof(DacStatus));
            }
        }

        /// <summary>
        /// The voltage value that is on the slider.
        /// </summary>
        public int SliderVoltageValue
        {
            get
            {
                return sliderVoltageValue;
            }
            set
            {
                sliderVoltageValue = value;
                VoltageValue = (float)sliderVoltageValue / 100;
                VoltageValueString = string.Format(voltageValue.ToString("0.##"));
                OnPropertyChanged(nameof(SliderVoltageValue));

                // Update DAC value when we scroll slider
                // SendDacValue();
            }
        }

        private string voltageValueString;

        public string VoltageValueString
        {
            get
            {
                return voltageValueString;
            }

            set
            {
                voltageValueString = value;
                if (float.TryParse(value, out var parsedVoltage))
                {
                    VoltageValue = parsedVoltage;
                }

                OnPropertyChanged(nameof(VoltageValueString));
            }
        }

        /// <summary>
        /// Gets or sets the voltage value to send.
        /// </summary>
        public float VoltageValue
        {
            get
            {
                return voltageValue;
            }

            set
            {
                if (value < 0)
                {
                    voltageValue = 0;
                }
                else if (value > 5)
                {
                    voltageValue = 5;
                }
                else
                {
                    voltageValue = value;
                }

                OnPropertyChanged(nameof(VoltageValue));

                sliderVoltageValue = (int)(voltageValue * 100);
                OnPropertyChanged(nameof(SliderVoltageValue));
                
            }
        }

        /// <summary>
        /// Sends the voltage value set.
        /// </summary>
        private async void SendDacValue()
        {
            await dacModel.SetDacCommand(VoltageValue);
        }

        /// <summary>
        /// Updates DAC.
        /// </summary>
        private void StartUpdateTask()
        {
            var task = Task.Factory.StartNew(() =>
            {
                CheckDacStatus();
            }, token);
        }

        /// <summary>
        /// Checks the DAC status.
        /// </summary>
        private async void CheckDacStatus()
        {
            while (true)
            {
                if (token.IsCancellationRequested == true)
                {
                    break;
                }

                try
                {
                    var status = await dacModel.ReadDacStatusCommand();

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
                            DacStatus = "Communication Error";
                        }));
                        
                    }
                    
                    Thread.Sleep(updateDelay);
                }
                catch (Exception e)
                {
                    DacStatus = e.Message;
                }
            }
        }

        /// <summary>
        /// Processes DAC register status.
        /// </summary>
        /// <param name="status"> DAC register response. </param>
        private void ProcessStatus(byte[] status)
        {
            if (status.Length < 4)
            {
                DacStatus = "Communication Error";
                return;
            }

            DacStatus = $"Status: {GetStatusMessage(status[4])}";
        }

        /// <summary>
        /// Gets the status message for given DAC response.
        /// </summary>
        /// <param name="value"> DAC register response. </param>
        /// <returns> Status for DAC. </returns>
        private string GetStatusMessage(byte value)
        {
            string response;
            DacDefaults.DacStatus.TryGetValue(value, out response);

            return response == null ? "Unknown Response" : response;
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
