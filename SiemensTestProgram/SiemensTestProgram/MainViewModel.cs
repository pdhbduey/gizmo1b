namespace SiemensTestProgram
{
    using System.Collections.ObjectModel;
    using System.Windows;

    using Common.Bindings;

    public class MainViewModel : BindableBase
    {
        private object content;
        private string selectedTestView;
        private bool configuring;

        public MainViewModel()
        {
            // Default settings
            configuring = false;
            ConfigureComCommand = new RelayCommand(param => ConfigureCommunication());
            ExitCommand = new RelayCommand(param => ReturnToSelectedView());

            TestViews = new ObservableCollection<string>()
            {
                "LED/Switch",
                "ADC",
                "DAC",
                "Motor",
                "Fan",
                "Thermistor",
                "TEC"
            };

            selectedTestView = TestViews[0];

            SetContent();
        }

        /// <summary>
        /// Changes to COM configuration menu.
        /// </summary>
        public RelayCommand ConfigureComCommand { get; set; }

        /// <summary>
        /// Command to return to currently set view.
        /// </summary>
        public RelayCommand ExitCommand { get; set; }

        /// <summary>
        /// Collection of all the available views.
        /// </summary>
        public ObservableCollection<string> TestViews { get; set; }

        /// <summary>
        /// Visibility of communication configuration.
        /// </summary>
        public Visibility CommunicationVisibility
        {
            get
            {
                if (configuring == true)
                {
                    return Visibility.Collapsed;
                }

                return Visibility.Visible;
            }
        }

        /// <summary>
        /// Visibility of return/configuration button.
        /// </summary>
        public Visibility ReturnVisibility
        {
            get
            {
                if (configuring == false)
                {
                    return Visibility.Collapsed;
                }

                return Visibility.Visible;
            }
        }

        /// <summary>
        /// The selected test view.
        /// </summary>
        public string SelectedTestView
        {
            get
            {
                return selectedTestView;
            }
            set
            {
                selectedTestView = value;
                SetContent();
            }
        }

        /// <summary>
        /// Content to display to view.
        /// </summary>
        public object Content
        {
            get
            {
                return content;
            }

            set
            {
                content = value;
                OnPropertyChanged(nameof(Content));
            }
        }

        /// <summary>
        /// Changes content of view in the main interface
        /// </summary>
        private void SetContent()
        {
            switch (selectedTestView)
            {
                case "LED/Switch":
                    Content = DeviceManager.Factory.Instance.GetLedView();
                    break;
                case "DAC":
                    Content = DeviceManager.Factory.Instance.GetDacView();
                    break;
                case "ADC":
                    Content = DeviceManager.Factory.Instance.GetAdcView();
                    break;
                case "Motor":
                    Content = DeviceManager.Factory.Instance.GetMotorView();
                    break;
                case "Fan":
                    Content = DeviceManager.Factory.Instance.GetFanView();
                    break;
                case "Thermistor":
                    Content = DeviceManager.Factory.Instance.GetThermistorView();
                    break;
                case "TEC":
                    Content = DeviceManager.Factory.Instance.GetTecView();
                    break;
                default:
                    return;
            }
        }

        /// <summary>
        /// Returns to currently set view from configuration view.
        /// </summary>
        private void ReturnToSelectedView()
        {
            UpdatingConfiguration(false);
            SetContent();
        }

        /// <summary>
        /// Sets the content to the COM configuration view.
        /// </summary>
        private void ConfigureCommunication()
        {
            UpdatingConfiguration(true);
            Content = DeviceManager.Factory.Instance.GetCommunicationConfigurationView();
        }

        /// <summary>
        /// Sets visibility parameters of layout based on whether configuration active.
        /// </summary>
        /// <param name="enabled"> Enable/Disables COM configuration. </param>
        private void UpdatingConfiguration(bool enabled)
        {
            configuring = enabled;
            OnPropertyChanged(nameof(CommunicationVisibility));
            OnPropertyChanged(nameof(ReturnVisibility));
        }
    }
}
