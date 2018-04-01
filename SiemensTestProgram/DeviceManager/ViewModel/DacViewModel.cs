// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common.Bindings;
    using DeviceManager.Model;

    public class DacViewModel : BindableBase
    {
        private IDacModel dacModel;
        private bool continuousMode;
        private float voltageValue;
        private int sliderVoltageValue;

        public DacViewModel(IDacModel dacModel)
        {
            this.dacModel = dacModel;
            ContinuousMode = true;
            SendDacValueCommand = new RelayCommand(param => SendDacValue());
        }

        public RelayCommand SendDacValueCommand { get; set; }

        public bool StepMode
        {
            get
            {
                return !continuousMode;
            }

            set
            {
                continuousMode = !value;
                OnPropertyChanged(nameof(ContinuousMode));
                OnPropertyChanged(nameof(StepMode));

                // reset to 0
                SliderVoltageValue = 250;
                SendDacValue();
            }
        }

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
                OnPropertyChanged(nameof(SliderVoltageValue));

                if (continuousMode)
                {
                    SendDacValue();
                }
            }
        }

        public bool ContinuousMode
        {
            get
            {
                return continuousMode;
            }
            set
            {                
                continuousMode = value;
                OnPropertyChanged(nameof(ContinuousMode));
                OnPropertyChanged(nameof(StepMode));

                // reset to 0
                SliderVoltageValue = 250;
                SendDacValue();
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
            }
        }

        /// <summary>
        /// Sends the voltage value set.
        /// </summary>
        private async void SendDacValue()
        {
            var response = await dacModel.WriteData(VoltageValue);
        }
    }
}
