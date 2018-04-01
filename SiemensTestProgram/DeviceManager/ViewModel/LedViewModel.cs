// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common;
    using Common.Bindings;
    using DeviceManager.Model;

    /// <summary>
    /// This class is responsible for updating LED and switch data
    /// </summary>
    public class LedViewModel : BindableBase
    {
        private ILedModel ledModel;
        private string MessageSent;
        private string messageReceived;
        private bool statusLedRed;
        private bool statusLedGreen;
        private string message;

        public LedViewModel(ILedModel ledModel)
        {
            this.ledModel = ledModel;
            MessageSent = string.Empty;
            MessageReceived = string.Empty;

            // Sets up the commands
            ToggleLedRedCommand = new RelayCommand(param => ToggleLedRed());
            ToggleLedGreenCommand = new RelayCommand(param => ToggleLedGreen());
            SendMessageCommand = new RelayCommand(param => SendMessage());
        }

        public string Message { get; set; }

        public string MessageReceived {
            get
            {
                return messageReceived;
            }
            set
            {
                messageReceived = value;
                OnPropertyChanged(nameof(MessageReceived));
            }
        }

        public RelayCommand ToggleLedRedCommand { get; set; }

        public RelayCommand ToggleLedGreenCommand { get; set; }

        public RelayCommand SendMessageCommand { get; set; }

        private void ToggleLedRed()
        {
            statusLedRed = statusLedRed == true ? false : true;
            switch (statusLedRed)
            {
                case true:
                    ToggleLed(DataHelper.REGISTER_WRITE, DataHelper.LED_RED_ON);
                    break;
                case false:
                    ToggleLed(DataHelper.REGISTER_WRITE, DataHelper.LED_RED_OFF);
                    break;
            }
        }

        private void ToggleLedGreen()
        {
            statusLedGreen = statusLedGreen == true ? false : true;
            switch (statusLedGreen)
            {
                case true:
                    ToggleLed(DataHelper.REGISTER_WRITE, DataHelper.LED_GREEN_ON);
                    break;
                case false:
                    ToggleLed(DataHelper.REGISTER_WRITE, DataHelper.LED_GREEN_OFF);
                    break;
            }
        }

        private async void ToggleLed(byte rw, byte led)
        {
            var request = ComCommands.GetLedCommand(rw, led);
            var response = await ledModel.WriteData(request);
            MessageReceived = DataHelper.ProcessCommandResponse(response, ComCommands.CommandType.LED);
        }

        private async void SendMessage()
        {
            if (string.IsNullOrEmpty(Message))
            {
                MessageReceived = "Message must be proper format";
            }

            var request = DataHelper.ParseStringToByteArray(Message);
            var response = await ledModel.WriteData(request);

            byte[] address = new byte[4]
            {
                request[1],
                request[2],
                request[3],
                request[4]
            };

            if (Helper.AreByteArraysEqual(address, ComCommands.LED_ADDRESS))
            {
                MessageReceived = DataHelper.ProcessCommandResponse(response, ComCommands.CommandType.LED);
            }
            else
            {
                MessageReceived = DataHelper.ProcessCommandResponse(response, ComCommands.CommandType.Unknown);
            }
        }
    }
}
