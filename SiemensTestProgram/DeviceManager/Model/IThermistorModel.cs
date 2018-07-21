// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceManager.DeviceCommunication;
    using System.Threading.Tasks;

    public interface IThermistorModel
    {
        Task<CommunicationData> ReadAinA();

        Task<CommunicationData> ReadAinB();

        Task<CommunicationData> ReadAinC();

        Task<CommunicationData> ReadAinD();

        Task<CommunicationData> ReadStatus();

        Task<CommunicationData> SetType(string type);
    }
}
