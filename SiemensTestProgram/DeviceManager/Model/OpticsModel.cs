
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
    }
}
