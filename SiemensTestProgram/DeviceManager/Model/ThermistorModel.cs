// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    using DeviceCommunication;

    public class ThermistorModel : IThermistorModel
    {
        private IComCommunication communication;

        public ThermistorModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<byte[]> ReadStatus()
        {
            var requestArray = ThermistorDefaults.GetStatusCommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }

        public Task<byte[]> ReadAinA()
        {
            var requestArray = ThermistorDefaults.GetReadAinACommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }

        public Task<byte[]> ReadAinB()
        {
            var requestArray = ThermistorDefaults.GetReadAinBCommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }

        public Task<byte[]> ReadAinC()
        {
            var requestArray = ThermistorDefaults.GetReadAinCCommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }

        public Task<byte[]> ReadAinD()
        {
            var requestArray = ThermistorDefaults.GetReadAinDCommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }
    }
}
