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

        Task<byte[]> Energize();

        Task<byte[]> Reset();

        Task<byte[]> Limp();

        void SetRelativeMovePosition(int relativePosition);

        void SetAbsoluteMovePosition(int absolutePosition);

        Task<byte[]> MoveRelative();

        Task<byte[]> MoveAbsolute();

        Task<byte[]> GetMotorPosition();

        void InitialSet();
    }
}
