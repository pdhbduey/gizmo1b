// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common.Bindings;
    using Model;

    public class AdcViewModel : BindableBase
    {
        private IAdcModel adcModel;
        private string testMvvmCommand;

        public AdcViewModel(IAdcModel adcModel)
        {
            this.adcModel = adcModel;
            testMvvmCommand = "ADC View";
        }

        public string TestMvvmCommand
        {
            get
            {
                return testMvvmCommand;
            }

            set
            {
                testMvvmCommand = value;
                OnPropertyChanged(nameof(TestMvvmCommand));
            }
        }
    }
}
