// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    using Common;
    using DeviceCommunication;

    public class AdcModel : IAdcModel
    {
        private IComCommunication communication;

        public AdcModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<CommunicationData> ReadStatus()
        {
            var requestArray = AdcDefaults.GetStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public bool ReadStatus(ref byte[] response)
        {
            var requestArray = AdcDefaults.GetStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public Task<CommunicationData> ControlAdcChannel(int channelNumber)
        {
            var requestArray = AdcDefaults.GetControlAdcCommand(channelNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public bool ControlAdcChannel(int channelNumber, ref byte[] response)
        {           
            var requestArray = AdcDefaults.GetControlAdcCommand(channelNumber);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }


        public Task<CommunicationData> ReadAdcResult()
        {
            var requestArray = AdcDefaults.GetReadAdcCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public bool ReadAdcResult(ref byte[] response)
        {
            var requestArray = AdcDefaults.GetReadAdcCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }
    }
}
