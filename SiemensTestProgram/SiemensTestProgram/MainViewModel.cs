namespace SiemensTestProgram
{
    using System.Collections.ObjectModel;
    using System.Windows;

    using Common.Bindings;

    public class MainViewModel : BindableBase
    {
        private object content;
        private string selectedTestView;

        public MainViewModel()
        {
            // Default settings
            TestViews = new ObservableCollection<string>()
            {
                "COM Port",
                "ADC",
                "DAC",
                "DIO",
                "Fan",
                "Fault",
                "LED",
                "Motor",
                "Thermistor",
                "Snapshot",
                //"Trace",
                "TEC"
            };

            selectedTestView = TestViews[0];

            SetContent();
        }

        /// <summary>
        /// Collection of all the available views.
        /// </summary>
        public ObservableCollection<string> TestViews { get; set; }
        
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
                case "COM Port":
                    Content = DeviceManager.Factory.Instance.GetCommunicationConfigurationView();
                    break;
                case "LED":
                    Content = DeviceManager.Factory.Instance.GetLedView();
                    break;
                case "DAC":
                    Content = DeviceManager.Factory.Instance.GetDacView();
                    break;
                case "DIO":
                    Content = DeviceManager.Factory.Instance.GetDioView();
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
                case "Fault":
                    Content = DeviceManager.Factory.Instance.GetFaultView();
                    break;
                case "Snapshot":
                    Content = DeviceManager.Factory.Instance.GetSnapshotView();
                    break;
                case "Trace":
                    Content = DeviceManager.Factory.Instance.GetTraceView();
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

            System.GC.Collect();
        }
    }
}
