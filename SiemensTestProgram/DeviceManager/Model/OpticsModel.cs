
namespace DeviceManager.Model
{
    using DeviceManager.DeviceCommunication;
    using System.Threading.Tasks;

    public class OpticsModel : IOpticsModel
    {
        private IComCommunication communication;

        public OpticsModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<CommunicationData> ReadStatusCommand()
        {
            var requestArray = OpticsDefault.ReadStatus();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetLedCommand(string led)
        {
            var requestArray = OpticsDefault.SetLedControlCommand(led);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetPhotodiodeCommand(string photodiode)
        {
            var requestArray = OpticsDefault.SetPhotodiodeControlCommand(photodiode);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetIntegrationTimeCommand(int integrationTime)
        {
            var requestArray = OpticsDefault.SetIntegrationTime(integrationTime);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetIntensityCommand(int intensity)
        {
            var requestArray = OpticsDefault.SetIntensity(intensity);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadPhotodiodeVoltsCommand()
        {
            var requestArray = OpticsDefault.ReadPhotodiodeVolts();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadPhotodiodeRawCommand()
        {
            var requestArray = OpticsDefault.ReadPhotodiodeRaw();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadLedTemperatureDuringIntegrationCommand()
        {
            var requestArray = OpticsDefault.ReadLedTemperatureDuringIntegrationCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadPdTemperatureDuringIntegrationCommand()
        {
            var requestArray = OpticsDefault.ReadPdTemperatureDuringIntegrationCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadLedTemperatureCommand()
        {
            var requestArray = OpticsDefault.ReadLedTemperatureCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadPdTemperatureCommand()
        {
            var requestArray = OpticsDefault.ReadPdTemperatureCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetPdBoardVersion(int version)
        {
            var requestArray = OpticsDefault.SetPdBoardVersionCommand(version);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetLedBoardVersion(int version)
        {
            var requestArray = OpticsDefault.SetLedBoardVersionCommand(version);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadLedMonitorVolts()
        {
            var requestArray = OpticsDefault.ReadLedMonitorVolts();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

    }
}
