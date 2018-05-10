// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    public interface ITecModel
    {
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

        bool SetIrefCommand(float irefGain, ref byte[] response);

        bool ReadWaveformIndex(ref byte[] response);

        bool Reset(ref byte[] response);
    }
}
