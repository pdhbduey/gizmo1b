

namespace DeviceManager.ViewModel
{
    using DeviceManager.View;

    public class MergedTecAndHeaterViewModel
    {
        public MergedTecAndHeaterViewModel(HeaterView heaterView, TecView tecView, FaultView faultView)
        {
            PassedHeaterView = heaterView;
            PassedTecView = tecView;
            PassedFaultView = faultView;
        }

        public object PassedHeaterView { get; set; }

        public object PassedTecView { get; set; }

        public object PassedFaultView { get; set; }
    }
}
