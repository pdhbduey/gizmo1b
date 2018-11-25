// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    using DeviceCommunication;

    public class DioModel : IDioModel
    {
        private IComCommunication communication;

        public DioModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<CommunicationData> ReadDin()
        {
            var request = DioDefaults.ReadDioInCommand();
            var status = communication.ProcessCommunicationRequest(request);
            return status;
        }

        public Task<CommunicationData> SetDout(int channel, bool set)
        {
            var request = DioDefaults.SetDioOutCommand(channel, set);
            var status = communication.ProcessCommunicationRequest(request);
            return status;
        }

        public Task<CommunicationData> ReadDout()
        {
            var request = DioDefaults.ReadDioOutCommand();
            var status = communication.ProcessCommunicationRequest(request);
            return status;
        }
    }
}
