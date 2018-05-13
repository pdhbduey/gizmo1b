// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using DeviceManager.DeviceCommunication;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IFaultModel
    {
        bool Reset(ref byte[] response);

        bool GetState(ref byte[] response);

        bool GetNtcStatus(ref byte[] response);

        Task<CommunicationData> GetState();

        Task<CommunicationData> GetNtcStatus();
    }
}
