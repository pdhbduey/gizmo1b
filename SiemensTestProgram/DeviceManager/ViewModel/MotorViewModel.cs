// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using DeviceManager.Model;

    public class MotorViewModel
    {
        private IMotorModel motorModel;

        public MotorViewModel(IMotorModel motorModel)
        {
            this.motorModel = motorModel;
        }
    }
}
