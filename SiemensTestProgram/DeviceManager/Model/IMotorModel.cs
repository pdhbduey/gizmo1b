// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using DeviceManager.DeviceCommunication;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IMotorModel
    {
        Task<CommunicationData> Cycle();

        Task<CommunicationData> Stop();

        //Task<byte[]> SetDirection(string selectedDirection);
        Task<CommunicationData> MotorControlMove(string selectedDirection, string selectedStepSize, string move);

        Task<CommunicationData> SetRegisterValue(byte[] selectedDirection);

        Task<CommunicationData> SetRegisterAddress(string address);

        Task<CommunicationData> ReadRegisterValue();

        Task<CommunicationData> Home();

        Task<CommunicationData> Initialize();

        Task<CommunicationData> Energize();

        Task<CommunicationData> Reset();

        Task<CommunicationData> Limp();


        Task<CommunicationData> SetRelativeMovePosition(int relativePosition);

        Task<CommunicationData> SetAbsoluteMovePosition(int absolutePosition);

        Task<CommunicationData> GetMotorPosition();

        Task<CommunicationData> GetMotorStatus();
    }
}
