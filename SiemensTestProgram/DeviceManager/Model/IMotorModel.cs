// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IMotorModel
    {
        Task<byte[]> Cycle();

        Task<byte[]> Stop();

        //Task<byte[]> SetDirection(string selectedDirection);
        Task<byte[]>  MotorControlMove(string selectedDirection, string selectedStepSize, string move);

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

        //Task<byte[]> MoveRelative();

        //Task<byte[]> MoveAbsolute();

        Task<byte[]> GetMotorPosition();

    }
}
