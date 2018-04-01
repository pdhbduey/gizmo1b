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

        Task<byte[]> SetPeriodCommand(int period);

        Task<byte[]> Reset();
    }
}
