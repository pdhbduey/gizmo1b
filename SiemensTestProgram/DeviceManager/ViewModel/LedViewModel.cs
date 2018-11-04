 // <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common;
    using Common.Bindings;
    using DeviceManager.Model;
    using System;

    /// <summary>
    /// This class is responsible for updating LED.
    /// </summary>
    public class LedViewModel : BindableBase, IDisposable
    {
        private ILedModel ledModel;
        private string ledStatus;
        private string greenLedStatus;
        private string redLedStatus;

        public LedViewModel(ILedModel ledModel)
        {
            this.ledModel = ledModel;
            ledStatus = string.Empty;
            redLedStatus = LedDefaults.redLedOn;
            greenLedStatus = LedDefaults.greenLedOn;

            InitialUpdate();

            // Sets up the commands
            ToggleLedRedCommand = new RelayCommand(param => ToggleLedRed());
            ToggleLedGreenCommand = new RelayCommand(param => ToggleLedGreen());
        }

        /// <summary>
        /// Toggle Red LED command.
        /// </summary>
        public RelayCommand ToggleLedRedCommand { get; set; }

        /// <summary>
        /// Toggle Green LED command.
        /// </summary>
        public RelayCommand ToggleLedGreenCommand { get; set; }

        /// <summary>
        /// LED status value.
        /// </summary>
        public string LedStatus {
            get
            {
                return ledStatus;
            }
            set
            {
                ledStatus = value;
                OnPropertyChanged(nameof(LedStatus));
            }
        }

        /// <summary>
        /// Green LED status value.
        /// </summary>
        public string GreenLedStatus
        {
            get
            {
                return greenLedStatus;
            }
            set
            {
                greenLedStatus = value;
                OnPropertyChanged(nameof(GreenLedStatus));
            }
        }

        /// <summary>
        /// Red LED status value.
        /// </summary>
        public string RedLedStatus
        {
            get
            {
                return redLedStatus;
            }
            set
            {
                redLedStatus = value;
                OnPropertyChanged(nameof(RedLedStatus));
            }
        }

        public bool GreenLedIsChecked
        {
            get => greenLedStatus.Equals(LedDefaults.greenLedOff);
        }

        public bool RedLedIsChecked
        {
            get => redLedStatus.Equals(LedDefaults.redLedOff);
        }

        private void InitialUpdate()
        {
            var ledStateResponse = ledModel.GetLedCommand().Result;

            if (ledStateResponse.succesfulResponse)
            {
                if (Helper.IsBitSet(ledStateResponse.response[4], 0))
                {
                    RedLedStatus = LedDefaults.redLedOff;
                }
                else
                {
                    RedLedStatus = LedDefaults.redLedOn;
                }

                if (Helper.IsBitSet(ledStateResponse.response[4], 2))
                {
                    GreenLedStatus = LedDefaults.greenLedOff;
                }
                else
                {
                    GreenLedStatus = LedDefaults.greenLedOn;
                }
            }
        }

        /// <summary>
        /// Toggles the red LED.
        /// </summary>
        private async void ToggleLedRed()
        {
            var state = redLedStatus;
            
            var status = await ledModel.SetLedCommand(state);
            if (status.succesfulResponse)
            {
                RedLedStatus = redLedStatus == LedDefaults.redLedOn ? LedDefaults.redLedOff : LedDefaults.redLedOn;
                ProcessStatus(status.response);
            }
            else
            {
                LedStatus = "Communication Error";
            }

            OnPropertyChanged(nameof(RedLedIsChecked));
        }

        /// <summary>
        /// Toggles the green LED.
        /// </summary>
        private async void ToggleLedGreen()
        {
            var state = greenLedStatus;
            
            var status = await ledModel.SetLedCommand(state);
            if (status.succesfulResponse)
            {
                GreenLedStatus = greenLedStatus == LedDefaults.greenLedOn ? LedDefaults.greenLedOff : LedDefaults.greenLedOn;
                ProcessStatus(status.response);
            }
            else
            {
                LedStatus = "Communication Error";
            }

            OnPropertyChanged(nameof(GreenLedIsChecked));
        }

        /// <summary>
        /// Processes LED register status.
        /// </summary>
        /// <param name="status"> LED register response. </param>
        private void ProcessStatus(byte[] status)
        {
            if (status.Length < 4)
            {
                LedStatus = "Communication Error";
                return;
            }

            LedStatus = $"Status: {GetErrorMessage(status[4])}";
        }

        /// <summary>
        /// Gets the error message for given LED response.
        /// </summary>
        /// <param name="value"> LED register response. </param>
        /// <returns> Status for LED. </returns>
        private string GetErrorMessage(byte value)
        {
            string response;
            LedDefaults.LedStatus.TryGetValue(value, out response);

            return response == null ? "Unknown Response" : response;
        }

        public void Dispose()
        {
            // do nothing
        }
    }
}
