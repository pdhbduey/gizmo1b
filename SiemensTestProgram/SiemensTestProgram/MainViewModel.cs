namespace SiemensTestProgram
{
    using Common.Bindings;
    using DeviceManager;
    using System;
    using System.Collections.ObjectModel;
    using System.Windows;

    public class MainViewModel : BindableBase
    {
        private object content;
        private string selectedTestView;
        private bool configuring;

        public MainViewModel()
        {
            configuring = false;
            ConfigureComCommand = new RelayCommand(param => ConfigureCommunication());
            ExitCommand = new RelayCommand(param => ReturnToSelectedView());

            TestViews = new ObservableCollection<string>()
            {
                "LED/Switch",
                //"ADC",
                "DAC",
                //"Motor",
                //"Fan",
                "Thermistor",
                "TEC"
            };

            selectedTestView = TestViews[0];

            SetContent();
        }

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

        public RelayCommand ConfigureComCommand { get; set; }

        public RelayCommand ExitCommand { get; set; }

        public ObservableCollection<string> TestViews { get; set; }

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
                case "Motor":
                    //Content = DeviceManager.Factory.Instance.GetMotorView();
                    break;
                case "Fan":
                    //Content = DeviceManager.Factory.Instance.GetFanView();
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

        private void ReturnToSelectedView()
        {
            UpdatingConfiguration(false);
            SetContent();
        }

        private void ConfigureCommunication()
        {
            UpdatingConfiguration(true);
            Content = DeviceManager.Factory.Instance.GetCommunicationConfigurationView();
        }

        private void UpdatingConfiguration(bool enabled)
        {
            configuring = enabled;
            OnPropertyChanged(nameof(CommunicationVisibility));
            OnPropertyChanged(nameof(ReturnVisibility));
        }
    }
}
