// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceManager.DeviceCommunication;
    using System.Threading.Tasks;

    public interface IThermistorModel
    {
        bool ReadAinA(ref byte[] response);

        bool ReadAinB(ref byte[] response);

        bool ReadAinC(ref byte[] response);

        bool ReadAinD(ref byte[] response);

        bool ReadStatus(ref byte[] response);

        Task<CommunicationData> ReadAinA();

        Task<CommunicationData> ReadAinB();

        Task<CommunicationData> ReadAinC();

        Task<CommunicationData> ReadAinD();

        Task<CommunicationData> ReadStatus();
    }
}
