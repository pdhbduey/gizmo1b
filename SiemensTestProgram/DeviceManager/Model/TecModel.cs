// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceCommunication;
    using System.Threading.Tasks;

    public class TecModel : ITecModel
    {
        private IComCommunication communication;
        private object mutex = new object();

        public TecModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<byte[]> SetIRef(byte[] data)
        {
            //lock (mutex)
            //{
                var requestArray = TecDefaults.GetSetIrefCommand(data);
                communication.WriteData(requestArray);
                return ReadStatus();
            //}
        }

        //public Task<byte[]> Reset()
        //{

        //}

        public Task<byte[]> ReadIsense()
        {
            //lock (mutex)
            //{
                var requestArray = TecDefaults.GetIsenseCommand();
                communication.WriteData(requestArray);
                var data = communication.ReadData();
                return data;
            //}
        }

        public Task<byte[]> ReadVsense()
        {
            //lock (mutex)
            //{
                var requestArray = TecDefaults.GetVsenseCommand();
                communication.WriteData(requestArray);
                var data = communication.ReadData();
                return data;
            //}
        }

        public Task<byte[]> ReadIref()
        {
            //lock (mutex)
            //{
                var requestArray = TecDefaults.GetIrefCommand();
                communication.WriteData(requestArray);
                var data = communication.ReadData();
                return data;
            //}
        }

        public Task<byte[]> ReadStatus()
        {
            //lock (mutex)
            //{
                var requestArray = TecDefaults.GetStatusCommand();
                communication.WriteData(requestArray);
                var data = communication.ReadData();
                return data;
            //}
        }

        public Task<byte[]> ControlCommand(string request)
        {
            //lock (mutex)
            //{
                var requestArray = TecDefaults.GetControlCommand(request);
                communication.WriteData(requestArray);
                return communication.ReadData(); // dummy read
                //return ReadStatus();
            //}
        }

        public Task<byte[]> SetWaveformCommand(string waveform)
        {
            //lock (mutex)
            //{
                var requestArray = TecDefaults.GetWaveformTypeCommand(waveform);
                communication.WriteData(requestArray);
                return communication.ReadData();
            //return ReadStatus();
            //}
        }

        public Task<byte[]> SetPeriodCommand(int period)
        {
            //lock (mutex)
            //{
                var requestArray = TecDefaults.GetWaveformPeriodCommand(period);
                communication.WriteData(requestArray);
                return communication.ReadData();
            //return ReadStatus();
            //}
        }
    }
}
