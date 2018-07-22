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

        bool ReadIsense(ref byte[] response);

        bool ReadVsense(ref byte[] response);

        bool ReadIref(ref byte[] response);

        bool ReadStatus(ref byte[] response);

        bool ControlCommand(string request, ref byte[] response);

        bool SetWaveformCommand(string waveform, ref byte[] response);

        bool SetWaveformCyclesCommand(int cycles, ref byte[] response);

        bool SetSampleTimeCommand(int sampleTime, ref byte[] response);

        bool SetPeriodCommand(int period, ref byte[] response);

        bool SetDerivativeGainCommand(float derivateGain, ref byte[] response);

        bool SetIntegralGainCommand(float integralGain, ref byte[] response);

        bool SetProportionalGainCommand(float proportionalGain, ref byte[] response);

        bool SetWaveformIrefCommand(float irefGain, ref byte[] response);

        Task<CommunicationData> SetVoutCommand(float value);

        Task<CommunicationData> SetSampleTimeCommand(int time);

        Task<CommunicationData> SetCustomWaveformIRefCommand(float iref);

        bool SetIrefCommand(float irefGain, ref byte[] response);

        bool ReadWaveformIndex(ref byte[] response);

        bool Reset(ref byte[] response);
    }
}
