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
        private SnapshotViewModel snapshotViewModel;
        private CommunicationConfigurationViewModel comConfiguration;

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
        public DacViewModel GetDacViewModel()
        {
            var vm = new DacViewModel(GetDacModel());
            return vm;
            //new DacView()
            //{
            //    DataContext = vm
            //};
        }

        public MergedTecAndHeaterViewModel GetFaultHeaterTecViewModel()
        {
            
            var heaterView = GetHeaterView();
            var tecView = GetTecView();
            var faultView = GetFaultView();
            var vm = new MergedTecAndHeaterViewModel(heaterView, tecView, faultView);
            return vm;
            //return new MergedTecAndHeaterView()
            //{
            //    DataContext = new MergedTecAndHeaterViewModel(heaterView, tecView, faultView)
            //};
        }

        /// <summary>
        ///  Sets the data context for Trace view.
        /// </summary>
        /// <returns> Trace view model</returns>
        public TraceViewModel GetTraceViewModel()
        {
            var vm = new TraceViewModel(GetTraceModel());
            return vm;
            //return new TraceView()
            //{
            //    DataContext = new TraceViewModel(GetTraceModel())
            //};
        }

        public ITraceModel GetTraceModel()
        {
            return new TraceModel(serialCommunication);
        }

        /// <summary>
        ///  Sets the data context for led view.
        /// </summary>
        /// <returns> LED view </returns>
        public LedViewModel GetLedViewModel()
        {
            var vm = new LedViewModel(GetLedModel());
            return vm;
            //return new LedView()
            //{
            //    DataContext = new LedViewModel(GetLedModel())
            //};
        }

        /// <summary>
        ///  Sets the data context for snapshot view.
        /// </summary>
        /// <returns> Snapshot view </returns>
        public SnapshotViewModel GetSnapshotViewModel()
        {
            if (snapshotViewModel == null)
            {
                snapshotViewModel = new SnapshotViewModel(GetSnapshotModel());
                //snapshotView = new SnapshotView()
                //{
                //    DataContext = snapshotViewModel
                //};
            }

            return snapshotViewModel;
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
        public ThermistorViewModel GetThermistorViewModel()
        {
            var vm = new ThermistorViewModel(GetThermistorModel());
            return vm;
            //return new ThermistorView()
            //    {
            //        DataContext = new ThermistorViewModel(GetThermistorModel())
            //    };

        }

        /// <summary>
        ///  Sets the data context for ADC view.
        /// </summary>
        /// <returns> ADC view </returns>
        public AdcViewModel GetAdcViewModel()
        {
            var vm = new AdcViewModel(GetAdcModel());
            return vm;
            //return new AdcView()
            //    {
            //        DataContext = new AdcViewModel(GetAdcModel())
            //    };
            
        }

        /// <summary>
        ///  Sets the data context for com configuration view.
        /// </summary>
        /// <returns> Communication configuration view. </returns>
        public CommunicationConfigurationViewModel GetCommunicationConfigurationViewModel()
        {
            //todo: differently???
            if (comConfiguration == null)
            {
                comConfiguration = new CommunicationConfigurationViewModel(GetCommunicationConfigurationModel());
                //comConfiguration = new CommunicationConfigurationView()
                //{
                //    DataContext = new CommunicationConfigurationViewModel(GetCommunicationConfigurationModel())
                //};
            }

            return comConfiguration;
        }

        /// <summary>
        ///  Sets the data context for fan view.
        /// </summary>
        /// <returns> Fan view. </returns>
        public FanViewModel GetFanViewModel()
        {
            var vm = new FanViewModel(GetFanModel());
            return vm;
            //return new FanView()
            //{
            //    DataContext = new FanViewModel(GetFanModel())
            //};
        }

        /// <summary>
        ///  Sets the data context for dio view.
        /// </summary>
        /// <returns> Dio view. </returns>
        public DioViewModel GetDioViewModel()
        {
            var vm = new DioViewModel(GetDioModel());
            return vm;
            //return new DioView()
            //{
            //    DataContext = new DioViewModel(GetDioModel())
            //};
        }

        /// <summary>
        ///  Sets the data context for motor view.
        /// </summary>
        /// <returns> Motor view. </returns>
        public MotorViewModel GetMotorViewModel()
        {
            var vm = new MotorViewModel(GetMotorModel());
            return vm;
            //return new MotorView()
            //{
            //    DataContext = new MotorViewModel(GetMotorModel())
            //};
            
        }

        /// <summary>
        ///  Sets the data context for Optics view.
        /// </summary>
        /// <returns> Optics view. </returns>
        public OpticsViewModel GetOpticsViewModel()
        {
            var vm = new OpticsViewModel(GetOpticsModel());
            return vm;
            //return new OpticsView()
            //{
            //    DataContext = new OpticsViewModel(GetOpticsModel())
            //};

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
