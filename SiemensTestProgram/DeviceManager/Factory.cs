// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using DeviceCommunication;
    using Model;
    using ViewModel;
    using View;

    public sealed class Factory
    {
        private static Factory instance = null;
        private IComCommunication serialCommunication;

        // Singleton Views
        private AdcView adcView;
        private MotorView motorView;
        private ThermistorView thermistorView;
        private FaultView faultView;
        private DacView dacView;
        private TecView tecView;
        private LedView ledView;

        // Singleton View Models
        private AdcViewModel adcViewModel;
        private MotorViewModel motorViewModel;
        private ThermistorViewModel thermistorViewModel;
        private FaultViewModel faultViewModel;
        private DacViewModel dacViewModel;
        private TecViewModel tecViewModel;
        private LedViewModel ledViewModel;

        /// <summary>
        /// Creates Device Manager Factory.
        /// </summary>
        public static Factory Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new Factory(); 
                    instance.serialCommunication = instance.GetComCommunication();
                }

                return instance;
            }
        }

        /// <summary>
        /// Creates serial port communication class.
        /// </summary>
        /// <returns> Com Communication </returns>
        private IComCommunication GetComCommunication()
        {
            return new ComCommunication();
        }

        /// <summary>
        ///  Sets the data context for DAC view.
        /// </summary>
        /// <returns> DAC view </returns>
        public DacView GetDacView()
        {
            if (dacViewModel == null)
            {
                dacViewModel = new DacViewModel(GetDacModel(serialCommunication));
                dacView = new DacView()
                {
                    DataContext = dacViewModel
                };
            }

            return dacView;
        }

        /// <summary>
        ///  Sets the data context for DAC view.
        /// </summary>
        /// <returns> DAC view </returns>
        public TraceView GetTraceView()
        {
            return new TraceView()
            {
                DataContext = new TraceViewModel(GetTraceModel())
            };
        }

        public ITraceModel GetTraceModel()
        {
            return new TraceModel(serialCommunication);
        }

        /// <summary>
        ///  Sets the data context for led view.
        /// </summary>
        /// <returns> LED view </returns>
        public LedView GetLedView()
        {
            if (ledViewModel == null)
            {
                ledViewModel = new LedViewModel(GetLedModel(serialCommunication));
                ledView = new LedView()
                {
                    DataContext = ledViewModel
                };
            }

            return ledView;
        }

        /// <summary>
        ///  Sets the data context for snapshot view.
        /// </summary>
        /// <returns> Snapshot view </returns>
        public SnapshotView GetSnapshotView()
        {
            return new SnapshotView()
            {
                DataContext = new SnapshotViewModel(GetSnapshotModel())
            };
        }

        ///// <summary>
        /////  Sets the data context for Capture view.
        ///// </summary>
        ///// <returns> Capture view </returns>
        //public CaptureView GetCaptureView()
        //{
        //    return new CaptureView()
        //    {
        //        DataContext = new CaptureViewModel(GetCaptureModel())
        //    };
        //}

        /// <summary>
        ///  Sets the data context for TEC view.
        /// </summary>
        /// <returns> TEC view </returns>
        public TecView GetTecView()
        {
            if (tecViewModel == null)
            {
                tecViewModel = new TecViewModel(GetTecModel(serialCommunication));
                tecView = new TecView()
                {
                    DataContext = tecViewModel
                };
            }

            return tecView;
        }

        /// <summary>
        ///  Sets the data context for thermistor view.
        /// </summary>
        /// <returns> Thermistor view </returns>
        public ThermistorView GetThermistorView()
        {
            if (thermistorViewModel == null)
            {
                thermistorViewModel = new ThermistorViewModel(GetThermistorModel());
                thermistorView = new ThermistorView()
                {
                    DataContext = thermistorViewModel
                };
            }

            return thermistorView;
        }

        /// <summary>
        ///  Sets the data context for ADC view.
        /// </summary>
        /// <returns> ADC view </returns>
        public AdcView GetAdcView()
        {
            if (adcViewModel == null)
            {
                adcViewModel = new AdcViewModel(GetAdcModel());
                adcView = new AdcView()
                {
                    DataContext = adcViewModel
                };
            }

            return adcView;
        }

        /// <summary>
        ///  Sets the data context for com configuration view.
        /// </summary>
        /// <returns> Communication configuration view. </returns>
        public CommunicationConfigurationView GetCommunicationConfigurationView()
        {
            return new CommunicationConfigurationView()
            {
                DataContext = new CommunicationConfigurationViewModel(GetCommunicationConfigurationModel(serialCommunication))
            };
        }

        /// <summary>
        ///  Sets the data context for fan view.
        /// </summary>
        /// <returns> Fan view. </returns>
        public FanView GetFanView()
        {
            return new FanView()
            {
                DataContext = new FanViewModel(GetFanModel())
            };
        }

        /// <summary>
        ///  Sets the data context for dio view.
        /// </summary>
        /// <returns> Dio view. </returns>
        public DioView GetDioView()
        {
            return new DioView()
            {
                DataContext = new DioViewModel(GetDioModel())
            };
        }

        /// <summary>
        ///  Sets the data context for motor view.
        /// </summary>
        /// <returns> Motor view. </returns>
        public MotorView GetMotorView()
        {
            if (motorViewModel == null)
            {
                motorViewModel = new MotorViewModel(GetMotorModel(serialCommunication));
                motorView = new MotorView()
                {
                    DataContext = motorViewModel
                };
            }

            return motorView;
        }

        /// <summary>
        ///  Sets the data context for fault view.
        /// </summary>
        /// <returns> Motor view. </returns>
        public FaultView GetFaultView()
        {
            if (faultViewModel == null)
            {
                faultViewModel = new FaultViewModel(GetFaultModel());
                faultView = new FaultView()
                {
                    DataContext = faultViewModel
                };
            }

            return faultView;
        }

        private LedModel GetLedModel(IComCommunication communication)
        {
            return new LedModel(communication);
        }

        private IDioModel GetDioModel()
        {
            return new DioModel(serialCommunication);
        }

        private IFaultModel GetFaultModel()
        {
            return new FaultModel(serialCommunication);
        }

        private IDacModel GetDacModel(IComCommunication communication)
        {
            return new DacModel(communication);
        }

        private ITecModel GetTecModel(IComCommunication communication)
        {
            return new TecModel(communication);
        }

        private ISnapshotModel GetSnapshotModel()
        {
            return new SnapshotModel(serialCommunication);
        }

        //private ICaptureModel GetCaptureModel()
        //{
        //    return new CaptureModel(serialCommunication);
        //}

        private IThermistorModel GetThermistorModel()
        {
            return new ThermistorModel(serialCommunication);
        }

        private IAdcModel GetAdcModel()
        {
            return new AdcModel(serialCommunication);
        }

        private IFanModel GetFanModel()
        {
            return new FanModel(serialCommunication);
        }

        private IMotorModel GetMotorModel(IComCommunication communication)
        {
            return new MotorModel(serialCommunication);
        }

        private ICommunicationConfigurationModel GetCommunicationConfigurationModel(IComCommunication communication)
        {
            return new CommunicationConfigurationModel(communication);
        }
    }
}
