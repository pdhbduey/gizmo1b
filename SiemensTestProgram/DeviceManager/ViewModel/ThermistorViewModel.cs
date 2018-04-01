// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common.Bindings;
    using DeviceManager.Model;

    public class ThermistorViewModel : BindableBase
    {
        private IThermistorModel thermistorModel;
        private string testMvvmBinding;

        public ThermistorViewModel(IThermistorModel thermistorModel)
        {
            this.thermistorModel = thermistorModel;
            testMvvmBinding = "Thermistor View";
        }

        public string TestMvvmBinding 
        {
            get
            {
                return testMvvmBinding;
            }

            set
            {
                testMvvmBinding = value;
                OnPropertyChanged(nameof(TestMvvmBinding));
            }
        }
    }
}
