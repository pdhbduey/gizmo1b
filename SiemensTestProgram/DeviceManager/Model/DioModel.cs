// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    using DeviceCommunication;

    public class DioModel : IDioModel
    {
        private IComCommunication communication;

        public DioModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public bool SetDout(int channel, bool set, ref byte[] response)
        {
            var request = DioDefaults.SetDioOutCommand(channel, set);
            var status = communication.ProcessCommunicationRequest(request, ref response);
            return status;
        }

        public bool ReadDin(ref byte[] response)
        {
            var request = DioDefaults.ReadDioInCommand();
            var status = communication.ProcessCommunicationRequest(request, ref response);
            return status;
        }
    }
}
