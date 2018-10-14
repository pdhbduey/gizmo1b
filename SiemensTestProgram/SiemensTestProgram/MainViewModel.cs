﻿// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->


namespace SiemensTestProgram
{
    using System.Collections.ObjectModel;
    using System.Threading;

    using Common.Bindings;

    /// <summary>
    /// This class is responsible for mapping viewmodels so correctly selected menu is shown.
    /// </summary>
    public class MainViewModel : BindableBase
    {
        private object content;
        private string selectedTestView;

        public MainViewModel()
        {
            TestViews = new ObservableCollection<string>()
            {
                "COM Port",
                "ADC",
                "DAC",
                "DIO",
                "Fan",
                "LED",
                "Motor",
                "Optics",
                "Snapshot",
                "Thermistor",
                "TEC/Heater/Fault"
                //"Trace",
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
                    Content = DeviceManager.Factory.Instance.GetCommunicationConfigurationViewModel();
                    break;
                case "LED":
                    Content = DeviceManager.Factory.Instance.GetLedViewModel();
                    break;
                case "DAC":
                    Content = DeviceManager.Factory.Instance.GetDacViewModel();
                    break;
                case "DIO":
                    Content = DeviceManager.Factory.Instance.GetDioViewModel();
                    break;
                case "ADC":
                    Content = DeviceManager.Factory.Instance.GetAdcViewModel();
                    break;
                case "Motor":
                    Content = DeviceManager.Factory.Instance.GetMotorViewModel();
                    break;
                case "Optics":
                    Content = DeviceManager.Factory.Instance.GetOpticsViewModel();
                    break;
                case "Fan":
                    Content = DeviceManager.Factory.Instance.GetFanViewModel();
                    break;
                case "Snapshot":
                    Content = DeviceManager.Factory.Instance.GetSnapshotViewModel();
                    break;
                case "Trace":
                    Content = DeviceManager.Factory.Instance.GetTraceViewModel();
                    break;
                case "Thermistor":
                    Content = DeviceManager.Factory.Instance.GetThermistorViewModel();
                    break;
                case "TEC/Heater/Fault":
                    Content = DeviceManager.Factory.Instance.GetFaultHeaterTecViewModel();
                    break;
                default:
                    return;
            }

            Thread.Sleep(500);
            System.GC.Collect();
        }
    }
}
