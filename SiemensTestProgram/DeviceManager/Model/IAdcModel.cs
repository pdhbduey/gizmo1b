// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceManager.DeviceCommunication;
    using System.Threading.Tasks;

    public interface IAdcModel
    {
        bool ReadStatus(ref byte[] response);

        Task<CommunicationData> ReadStatus();

        Task<CommunicationData> ReadAdcResult();

        Task<CommunicationData> ControlAdcChannel(int channelNumber);

        bool ControlAdcChannel(int channelNumber, ref byte[] response);

        bool ReadAdcResult(ref byte[] response);
    }
}
