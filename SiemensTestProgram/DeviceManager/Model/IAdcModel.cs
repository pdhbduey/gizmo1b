// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    public interface IAdcModel
    {
        Task<byte[]> ReadStatus();

        Task<byte[]> ControlAdcChannel(int channelNumber);

        Task<byte[]> ReadAdcResult();
    }
}
