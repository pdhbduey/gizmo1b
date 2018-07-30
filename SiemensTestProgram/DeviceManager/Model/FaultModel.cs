// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceCommunication;
    using System.Threading.Tasks;

    public class FaultModel : IFaultModel
    {
        private IComCommunication communication;

        public FaultModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<CommunicationData> Reset()
        {
            var requestArray = FaultDefaults.ResetCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> GetState()
        {
            var requestArray = FaultDefaults.ReadStateCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> GetNtcStatus()
        {
            var requestArray = FaultDefaults.ReadNtcCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
