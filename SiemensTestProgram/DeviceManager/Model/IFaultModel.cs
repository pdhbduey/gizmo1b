// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IFaultModel
    {
        Task<byte[]> Reset();

        byte[] GetState();

        byte[] GetNtcStatus();
    }
}
