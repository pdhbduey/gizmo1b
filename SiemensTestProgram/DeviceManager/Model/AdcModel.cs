// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceCommunication;

    public class AdcModel : IAdcModel
    {
        private IComCommunication communication;

        public AdcModel(IComCommunication communication)
        {
            this.communication = communication;
        }
    }
}
