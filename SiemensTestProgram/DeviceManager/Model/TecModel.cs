// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceCommunication;
    using System.Threading.Tasks;

    public class TecModel : ITecModel
    {
        private IComCommunication communication;

        public TecModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        //public Task<byte[]> SetIRef(byte[] data)
        //{
        //    var requestArray = TecDefaults.GetSetIrefCommand(data);
        //    communication.WriteData(requestArray);
        //    var response = communication.ReadData();
        //    return response;
        //}

        public Task<byte[]> Reset()
        {
            var requestArray = TecDefaults.GetResetCommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }

        public Task<byte[]> ReadIsense()
        {
            var requestArray = TecDefaults.GetIsenseCommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }

        public Task<byte[]> ReadVsense()
        {
            var requestArray = TecDefaults.GetVsenseCommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }

        public Task<byte[]> ReadIref()
        {
            var requestArray = TecDefaults.GetIrefCommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }

        public Task<byte[]> ReadStatus()
        {
            var requestArray = TecDefaults.GetStatusCommand();
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }

        public Task<byte[]> ControlCommand(string request)
        {
            var requestArray = TecDefaults.GetControlCommand(request);
            communication.WriteData(requestArray);
            return communication.ReadData();
        }

        public Task<byte[]> SetWaveformCommand(string waveform)
        {
            var requestArray = TecDefaults.GetWaveformTypeCommand(waveform);
            communication.WriteData(requestArray);
            return communication.ReadData();
        }

        public Task<byte[]> SetPeriodCommand(int period)
        {
            var requestArray = TecDefaults.GetWaveformPeriodCommand(period);
            communication.WriteData(requestArray);
            return communication.ReadData();
        }

        public Task<byte[]> SetDerivativeGainCommand(int derivateGain)
        {
            var requestArray = TecDefaults.GetDerivateGainCommand(derivateGain);
            communication.WriteData(requestArray);
            return communication.ReadData();
        }

        public Task<byte[]> SetIntegralGainCommand(int integralGain)
        {
            var requestArray = TecDefaults.GetIntegralGainCommand(integralGain);
            communication.WriteData(requestArray);
            return communication.ReadData();
        }

        public Task<byte[]> SetProportionalGainCommand(float proportionalGain)
        {
            var requestArray = TecDefaults.GetProportionalGainCommand(proportionalGain);
            communication.WriteData(requestArray);
            return communication.ReadData();
        }

        //public Task<byte[]> SetIrefGainCommand(int irefGain)
        //{
        //    var requestArray = TecDefaults.GetIrefGainCommand(irefGain);
        //    communication.WriteData(requestArray);
        //    return communication.ReadData();
        //}
    }
}
