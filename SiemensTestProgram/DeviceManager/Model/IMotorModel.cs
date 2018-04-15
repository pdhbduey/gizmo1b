// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IMotorModel
    {
        Task<byte[]> SetStepMode(string selectedMode);

        Task<byte[]> SetDirection(string selectedDirection);

        Task<byte[]> SetRegisterValue(byte[] selectedDirection);

        Task<byte[]> SetRegisterAddress(string address);

        Task<byte[]> ReadRegisterValue();

        Task<byte[]> Home();

        Task<byte[]> Initialize();

        Task<byte[]> Reset();

        Task<byte[]> Limp();

        Task<byte[]> MoveRelative(int relativeMoveValue);

        Task<byte[]> MoveAbsolute(int absoluteMoveValue);
    }
}
