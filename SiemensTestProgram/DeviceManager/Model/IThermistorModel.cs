// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    public interface IThermistorModel
    {
        Task<byte[]> ReadAinA();

        Task<byte[]> ReadAinB();

        Task<byte[]> ReadAinC();

        Task<byte[]> ReadAinD();

        Task<byte[]> ReadStatus();
    }
}
