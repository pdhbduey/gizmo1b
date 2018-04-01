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

            return new DacView()
            {
                DataContext = new DacViewModel(GetDacModel(serialCommunication))
            };
        }

        /// <summary>
        ///  Sets the data context for led view.
        /// </summary>
        /// <returns> LED view </returns>
        public LedView GetLedView()
        {
            return new LedView()
            {
                DataContext = new LedViewModel(GetLedModel(serialCommunication))
            };
        }

        /// <summary>
        ///  Sets the data context for TEC view.
        /// </summary>
        /// <returns> TEC view </returns>
        public TecView GetTecView()
        {
            return new TecView()
            {
                DataContext = new TecViewModel(GetTecModel(serialCommunication))
            };
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

        private IMotorModel GetMotorModel(IComCommunication communication)
        {
            return new MotorModel(communication);
        }

        private ICommunicationConfigurationModel GetCommunicationConfigurationModel(IComCommunication communication)
        {
            return new CommunicationConfigurationModel(communication);
        }
    }
}
