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

        public Task<byte[]> ReadStatus()
        {
            var requestArray = AdcDefaults.GetStatusCommand();
            communication.WriteData(requestArray);
            var status = communication.ReadData();
            return status;
        }

        public Task<byte[]> ControlAdcChannel(int channelNumber)
        {           
            var requestArray = AdcDefaults.GetControlAdcCommand(channelNumber);
            communication.WriteData(requestArray);
            var status = communication.ReadData();
            return status;
        }

        public Task<byte[]> ReadAdcResult()
        {
            var requestArray = AdcDefaults.GetReadAdcCommand();
            communication.WriteData(requestArray);
            var status = communication.ReadData();
            return status;
        }
    }
}
