 // <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common.Bindings;
    using DeviceManager.Model;

    /// <summary>
    /// This class is responsible for updating LED.
    /// </summary>
    public class LedViewModel : BindableBase
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

        /// <summary>
        /// Toggles the red LED.
        /// </summary>
        private async void ToggleLedRed()
        {
            var state = redLedStatus;
            RedLedStatus = redLedStatus == LedDefaults.redLedOn ? LedDefaults.redLedOff : LedDefaults.redLedOn;
            var status = new byte[5];
            if (ledModel.SetLedCommand(state, ref status))
            {
                ProcessStatus(status);
            }
            else
            {
                LedStatus = "Communication Error";
            }
        }

        /// <summary>
        /// Toggles the green LED.
        /// </summary>
        private async void ToggleLedGreen()
        {
            var state = greenLedStatus;
            GreenLedStatus = greenLedStatus == LedDefaults.greenLedOn ? LedDefaults.greenLedOff : LedDefaults.greenLedOn;
            var status = new byte[5];
            if (ledModel.SetLedCommand(state, ref status))
            {
                ProcessStatus(status);
            }
            else
            {
                LedStatus = "Communication Error";
            }
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
    }
}
