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

        // Singletons
        private SnapshotView snapshotView;
        private SnapshotViewModel snapshotViewModel;

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
           return new DacView()
            {
                DataContext = new DacViewModel(GetDacModel())
            };
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
            return new LedView()
            {
                DataContext = new LedViewModel(GetLedModel())
            };
        }

        /// <summary>
        ///  Sets the data context for snapshot view.
        /// </summary>
        /// <returns> Snapshot view </returns>
        public SnapshotView GetSnapshotView()
        {
            if (snapshotViewModel == null)
            {
                snapshotViewModel = new SnapshotViewModel(GetSnapshotModel());
                snapshotView = new SnapshotView()
                {
                    DataContext = snapshotViewModel
                };
            }

            return snapshotView;
        }

        /// <summary>
        ///  Sets the data context for TEC view.
        /// </summary>
        /// <returns> TEC view </returns>
        public TecView GetTecView()
        {
            return new TecView()
                {
                    DataContext = new TecViewModel(GetTecModel())
                };
        }

        /// <summary>
        ///  Sets the data context for thermistor view.
        /// </summary>
        /// <returns> Thermistor view </returns>
        public ThermistorView GetThermistorView()
        {
            return new ThermistorView()
                {
                    DataContext = new ThermistorViewModel(GetThermistorModel())
                };

        }

        /// <summary>
        ///  Sets the data context for ADC view.
        /// </summary>
        /// <returns> ADC view </returns>
        public AdcView GetAdcView()
        {
            return new AdcView()
                {
                    DataContext = new AdcViewModel(GetAdcModel())
                };
            
        }

        /// <summary>
        ///  Sets the data context for com configuration view.
        /// </summary>
        /// <returns> Communication configuration view. </returns>
        public CommunicationConfigurationView GetCommunicationConfigurationView()
        {
            return new CommunicationConfigurationView()
            {
                DataContext = new CommunicationConfigurationViewModel(GetCommunicationConfigurationModel())
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
           return new MotorView()
            {
                DataContext = new MotorViewModel(GetMotorModel())
            };
            
        }

        /// <summary>
        ///  Sets the data context for Optics view.
        /// </summary>
        /// <returns> Optics view. </returns>
        public OpticsView GetOpticsView()
        {
            return new OpticsView()
            {
                DataContext = new OpticsViewModel(GetOpticsModel())
            };

        }

        /// <summary>
        ///  Sets the data context for fault view.
        /// </summary>
        /// <returns> Motor view. </returns>
        public FaultView GetFaultView()
        {
            return new FaultView()
                {
                    DataContext = new FaultViewModel(GetFaultModel())
                };
        }

        /// <summary>
        ///  Sets the data context for Heater view.
        /// </summary>
        /// <returns> Motor view. </returns>
        public HeaterView GetHeaterView()
        {
            return new HeaterView()
            {
                DataContext = new HeaterViewModel(GetHeaterModel())
            };
        }

        private IHeaterModel GetHeaterModel()
        {
            return new HeaterModel(serialCommunication);
        }

        private ILedModel GetLedModel()
        {
            return new LedModel(serialCommunication);
        }

        private IDioModel GetDioModel()
        {
            return new DioModel(serialCommunication);
        }

        private IFaultModel GetFaultModel()
        {
            return new FaultModel(serialCommunication);
        }

        private IDacModel GetDacModel()
        {
            return new DacModel(serialCommunication);
        }

        private ITecModel GetTecModel()
        {
            return new TecModel(serialCommunication);
        }

        private IOpticsModel GetOpticsModel()
        {
            return new OpticsModel(serialCommunication);
        }

        private ISnapshotModel GetSnapshotModel()
        {
            return new SnapshotModel(serialCommunication);
        }

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

        private IMotorModel GetMotorModel()
        {
            return new MotorModel(serialCommunication);
        }

        private ICommunicationConfigurationModel GetCommunicationConfigurationModel()
        {
            return new CommunicationConfigurationModel(serialCommunication);
        }
    }
}
