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

        public bool SetWaveformIrefCommand(float value, ref byte[] response)
        {
            var requestArray = TecDefaults.SetWaveformIrefCommand(value);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool SetIrefCommand(float value, ref byte[] response)
        {
            var requestArray = TecDefaults.SetIrefCommand(value);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadWaveformIndex(ref byte[] response)
        {
            var requestArray = TecDefaults.ReadWaveformIndex();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool Reset(ref byte[] response)
        {
            var requestArray = TecDefaults.GetResetCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadIsense(ref byte[] response)
        {
            var requestArray = TecDefaults.GetIsenseCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadVsense(ref byte[] response)
        {
            var requestArray = TecDefaults.GetVsenseCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadIref(ref byte[] response)
        {
            var requestArray = TecDefaults.GetIrefCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadStatus(ref byte[] response)
        {
            var requestArray = TecDefaults.GetStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ControlCommand(string request, ref byte[] response)
        {
            var requestArray = TecDefaults.GetControlCommand(request);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool SetWaveformCommand(string waveform, ref byte[] response)
        {
            var requestArray = TecDefaults.GetWaveformTypeCommand(waveform);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool SetSampleTimeCommand(int sampleTime, ref byte[] response)
        {
            var requestArray = TecDefaults.GetSampleTimeCommand(sampleTime);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool SetWaveformCyclesCommand(int cycles, ref byte[] response)
        {
            var requestArray = TecDefaults.GetWaveNumberOfCyclesCommand(cycles);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool SetPeriodCommand(int period, ref byte[] response)
        {
            var requestArray = TecDefaults.GetWaveformPeriodCommand(period);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool SetDerivativeGainCommand(float derivateGain, ref byte[] response)
        {
            var requestArray = TecDefaults.GetDerivateGainCommand(derivateGain);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool SetIntegralGainCommand(float integralGain, ref byte[] response)
        {
            var requestArray = TecDefaults.GetIntegralGainCommand(integralGain);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool SetProportionalGainCommand(float proportionalGain, ref byte[] response)
        {
            var requestArray = TecDefaults.GetProportionalGainCommand(proportionalGain);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public Task<CommunicationData> SetSampleTimeCommand(int time)
        {
            var requestArray = TecDefaults.GetSampleTimeCommand(time);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetCustomWaveformIRefCommand(float iref)
        {
            var requestArray = TecDefaults.SetWaveformIrefCommand(iref);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        //public Task<byte[]> SetIrefGainCommand(int irefGain)
        //{
        //    var requestArray = TecDefaults.GetIrefGainCommand(irefGain);
        //    communication.WriteData(requestArray);
        //    return communication.ReadData();
        //}
    }
}
