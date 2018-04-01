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
        private DacView dacView;
        private TecView tecView;
        private LedView ledView;
        private ThermistorView thermistorView;
        private AdcView adcView;

        // Singleton View Models
        private DacViewModel dacViewModel;
        private TecViewModel tecViewModel;
        private LedViewModel ledViewModel;
        private ThermistorViewModel thermistorViewModel;
        private AdcViewModel adcViewModel;

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
        /// Sets the data context for device manager view.
        /// </summary>
        /// <returns> Device Manager View </returns>
        public DeviceManagerView GetDeviceManagerView()
        {
            return new DeviceManagerView()
            {
                DataContext = new DeviceManagerViewModel(GetDeviceManager())
            };
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

        public CommunicationConfigurationView GetCommunicationConfigurationView()
        {
            return new CommunicationConfigurationView()
            {
                DataContext = new CommunicationConfigurationViewModel(GetCommunicationConfigurationModel(serialCommunication))
            };
        } 

        public MotorView GetMotorView()
        {
            return new MotorView()
            {
                DataContext = new MotorViewModel(GetMotorModel(serialCommunication))
            };
        }

        private DeviceManager GetDeviceManager()
        {
            return new DeviceManager();
        }

        private LedModel GetLedModel(IComCommunication communication)
        {
            return new LedModel(communication);
        }

        private IDacModel GetDacModel(IComCommunication communication)
        {
            return new DacModel(communication);
        }

        private ITecModel GetTecModel(IComCommunication communication)
        {
            return new TecModel(communication);
        }

        private IThermistorModel GetThermistorModel()
        {
            return new ThermistorModel(serialCommunication);
        }

        private IAdcModel GetAdcModel()
        {
            return new AdcModel(serialCommunication);
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
