// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using DeviceManager.DeviceCommunication;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IFaultModel
    {
        Task<CommunicationData> Reset();

        Task<CommunicationData> GetState();

        Task<CommunicationData> GetNtcStatus();
    }
}
