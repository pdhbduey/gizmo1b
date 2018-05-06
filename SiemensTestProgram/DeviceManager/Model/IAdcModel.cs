// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    public interface IAdcModel
    {
        bool ReadStatus(ref byte[] response);

        bool ControlAdcChannel(int channelNumber, ref byte[] response);

        bool ReadAdcResult(ref byte[] response);
    }
}
