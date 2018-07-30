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

        public Task<CommunicationData> ReadControlState()
        {
            var requestArray = TecDefaults.ReadControlCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadVmax()
        {
            var requestArray = TecDefaults.GetVmaxCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadIrefSamples()
        {
            var requestArray = TecDefaults.GetIrefSamplesCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> ReadCycles()
        {
            var requestArray = TecDefaults.GetReadCyclesCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadPeriod()
        {
            var requestArray = TecDefaults.GetWaveformPeriodCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> ReadIntegralGain()
        {
            var requestArray = TecDefaults.GetIntegralGainCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> ReadDerivativeGain()
        {
            var requestArray = TecDefaults.GetDerivativeGainCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> ReadProportionalGain()
        {
            var requestArray = TecDefaults.GetProportionalGainCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> ReadWaveform()
        {
            var requestArray = TecDefaults.GetWaveformTypeCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> SetWaveformIrefCommand(float value)
        {
            var requestArray = TecDefaults.SetWaveformIrefCommand(value);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetVoutCommand(float value)
        {
            var requestArray = TecDefaults.SetVoutCommand(value);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadIsense()
        {
            var requestArray = TecDefaults.GetIsenseCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadVsense()
        {
            var requestArray = TecDefaults.GetVsenseCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadIref()
        {
            var requestArray = TecDefaults.GetIrefCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadStatus()
        {
            var requestArray = TecDefaults.GetStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ControlCommand(string request)
        {
            var requestArray = TecDefaults.GetControlCommand(request);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetWaveformCommand(string waveform)
        {
            var requestArray = TecDefaults.SetWaveformTypeCommand(waveform);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetWaveformCyclesCommand(int cycles)
        {
            var requestArray = TecDefaults.SetWaveNumberOfCyclesCommand(cycles);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetPeriodCommand(int period)
        {
            var requestArray = TecDefaults.SetWaveformPeriodCommand(period);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetDerivativeGainCommand(float derivateGain)
        {
            var requestArray = TecDefaults.SetDerivateGainCommand(derivateGain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetIntegralGainCommand(float integralGain)
        {
            var requestArray = TecDefaults.SetIntegralGainCommand(integralGain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetProportionalGainCommand(float proportionalGain)
        {
            var requestArray = TecDefaults.SetProportionalGainCommand(proportionalGain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetSampleTimeCommand(int time)
        {
            var requestArray = TecDefaults.SetSampleTimeCommand(time);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetCustomWaveformIRefCommand(float iref)
        {
            var requestArray = TecDefaults.SetWaveformIrefCommand(iref);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetIrefCommand(float irefGain)
        {
            var requestArray = TecDefaults.SetIrefCommand(irefGain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> ReadWaveformIndex()
        {
            var requestArray = TecDefaults.ReadWaveformIndex();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> Reset()
        {
            var requestArray = TecDefaults.GetResetCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
