// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    using DeviceCommunication;

    public class ThermistorModel : IThermistorModel
    {
        private IComCommunication communication;

        public ThermistorModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public bool ReadStatus(ref byte[] response)
        {
            var requestArray = ThermistorDefaults.GetStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadAinA(ref byte[] response)
        {
            var requestArray = ThermistorDefaults.GetReadAinACommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadAinB(ref byte[] response)
        {
            var requestArray = ThermistorDefaults.GetReadAinBCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadAinC(ref byte[] response)
        {
            var requestArray = ThermistorDefaults.GetReadAinCCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadAinD(ref byte[] response)
        {
            var requestArray = ThermistorDefaults.GetReadAinDCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public Task<CommunicationData> ReadAinA()
        {
            var requestArray = ThermistorDefaults.GetReadAinACommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadAinB()
        {
            var requestArray = ThermistorDefaults.GetReadAinBCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadAinC()
        {
            var requestArray = ThermistorDefaults.GetReadAinCCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadAinD()
        {
            var requestArray = ThermistorDefaults.GetReadAinDCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadStatus()
        {
            var requestArray = ThermistorDefaults.GetStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
