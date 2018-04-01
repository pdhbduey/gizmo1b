// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface ITecModel
    {
        Task<byte[]> ReadIsense();

        Task<byte[]> ReadVsense();

        Task<byte[]> ReadIref();

        Task<byte[]> ReadStatus();

        Task<byte[]> ControlCommand(string request);

        Task<byte[]> SetWaveformCommand(string waveform);

        Task<byte[]> SetPeriodCommand(int period);
    }
}
