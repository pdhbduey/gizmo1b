// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceCommunication;

    public class FaultModel : IFaultModel
    {
        private IComCommunication communication;

        public FaultModel(IComCommunication communication)
        {
            this.communication = communication;
        }
    }
}
