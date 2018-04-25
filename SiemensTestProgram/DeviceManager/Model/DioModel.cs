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

        public Task<byte[]> SetDout(int channel, bool set)
        {
            var request = DioDefaults.SetDataOutByteArray(channel, set);
            communication.WriteData(request);
            return communication.ReadData();
        }

        public Task<byte[]> ReadDin()
        {
            var request = DioDefaults.ReadDioInCommand();
            communication.WriteData(request);
            return communication.ReadData();
        }
    }
}
