// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    public interface ITecModel
    {
        Task<byte[]> ReadIsense();

        Task<byte[]> ReadVsense();

        Task<byte[]> ReadIref();

        Task<byte[]> ReadStatus();

        Task<byte[]> ControlCommand(string request);

        Task<byte[]> SetWaveformCommand(string waveform);

        Task<byte[]> SetWaveformCyclesCommand(int cycles);

        Task<byte[]> SetSampleTimeCommand(int sampleTime);

        Task<byte[]> SetPeriodCommand(int period);

        Task<byte[]> SetDerivativeGainCommand(int derivateGain);

        Task<byte[]> SetIntegralGainCommand(int integralGain);

        Task<byte[]> SetProportionalGainCommand(float proportionalGain);

        Task<byte[]> SetIrefCommand(float irefGain);

        Task<byte[]> Reset();
    }
}
