// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IMotorModel
    {
        bool Cycle(ref byte[] response);

        bool Stop(ref byte[] response);

        //Task<byte[]> SetDirection(string selectedDirection);
        bool MotorControlMove(string selectedDirection, string selectedStepSize, string move, ref byte[] response);

        bool SetRegisterValue(byte[] selectedDirection, ref byte[] response);

        bool SetRegisterAddress(string address, ref byte[] response);

        bool ReadRegisterValue(ref byte[] response);

        bool Home(ref byte[] response);

        bool Initialize(ref byte[] response);

        bool Energize(ref byte[] response);

        bool Reset(ref byte[] response);

        bool Limp(ref byte[] response);


        void SetRelativeMovePosition(int relativePosition, ref byte[] response);

        void SetAbsoluteMovePosition(int absolutePosition, ref byte[] response);



        //Task<byte[]> MoveRelative();

        //Task<byte[]> MoveAbsolute();

        bool GetMotorPosition(ref byte[] response);

        bool GetMotorStatus(ref byte[] response);
    }
}
