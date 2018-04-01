// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceCommunication;

    public class ThermistorModel : IThermistorModel
    {
        private IComCommunication communication;

        public ThermistorModel(IComCommunication communication)
        {
            this.communication = communication;
        }
    }
}
