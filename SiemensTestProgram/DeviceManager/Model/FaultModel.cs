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

        public Task<byte[]> Reset()
        {
            var requestArray = FaultDefaults.ResetCommand();
            communication.WriteData(requestArray);
            return communication.ReadData();
        }

        public byte[] GetState()
        {
            var requestArray = FaultDefaults.ReadStateCommand();
            communication.WriteData(requestArray);
            return communication.ReadData().Result;
        }

        public byte[] GetNtcStatus()
        {
            var requestArray = FaultDefaults.ReadNtcCommand();
            communication.WriteData(requestArray);
            return communication.ReadData().Result;
        }
    }
}
