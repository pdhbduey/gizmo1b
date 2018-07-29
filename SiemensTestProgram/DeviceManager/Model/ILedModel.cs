// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceManager.DeviceCommunication;
    using System.Threading.Tasks;

    /// <summary>
    /// Interface for LedModel class.
    /// </summary>
    public interface ILedModel
    {
        Task<CommunicationData> SetLedCommand(string ledRequest);

        Task<CommunicationData> GetLedCommand();
    }
}
