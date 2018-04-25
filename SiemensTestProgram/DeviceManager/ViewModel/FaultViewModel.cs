// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common.Bindings;
    using Model;

    public class FaultViewModel : BindableBase
    {
        private IFaultModel faultModel;

        public FaultViewModel(IFaultModel faultModel)
        {
            this.faultModel = faultModel;
        }
    }
}
