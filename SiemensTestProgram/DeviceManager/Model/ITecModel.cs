// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceManager.DeviceCommunication;
    using System.Threading.Tasks;

    public interface ITecModel
    {
        Task<CommunicationData> ReadVmax();

        Task<CommunicationData> ReadIrefSamples();

        Task<CommunicationData> ReadCycles();

        Task<CommunicationData> ReadPeriod();

        Task<CommunicationData> ReadIntegralGain();

        Task<CommunicationData> ReadDerivativeGain();

        Task<CommunicationData> ReadProportionalGain();

        Task<CommunicationData> ReadWaveform();

        Task<CommunicationData> ReadControlState();

        Task<CommunicationData> ReadIsense();

        Task<CommunicationData> ReadVsense();

        Task<CommunicationData> ReadIref();

        Task<CommunicationData> ReadStatus();

        Task<CommunicationData> ControlCommand(string request);

        Task<CommunicationData> SetWaveformCommand(string waveform);

        Task<CommunicationData> SetWaveformCyclesCommand(int cycles);

        Task<CommunicationData> SetSampleTimeCommand(int sampleTime);

        Task<CommunicationData> SetPeriodCommand(int period);

        Task<CommunicationData> SetDerivativeGainCommand(float derivateGain);

        Task<CommunicationData> SetIntegralGainCommand(float integralGain);

        Task<CommunicationData> SetProportionalGainCommand(float proportionalGain);

        Task<CommunicationData> SetWaveformIrefCommand(float irefGain);

        Task<CommunicationData> SetVoutCommand(float value);

        Task<CommunicationData> SetCustomWaveformIRefCommand(float iref);

        Task<CommunicationData> SetIrefCommand(float irefGain);

        Task<CommunicationData> ReadWaveformIndex();

        Task<CommunicationData> Reset();

    }
}
