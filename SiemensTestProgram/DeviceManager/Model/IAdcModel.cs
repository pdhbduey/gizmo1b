// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceManager.DeviceCommunication;
    using System.Threading.Tasks;

    public interface IAdcModel
    {
        Task<CommunicationData> ReadStatus();

        Task<CommunicationData> ReadAdcResult();

        Task<CommunicationData> ControlAdcChannel(int channelNumber);
    }
}
