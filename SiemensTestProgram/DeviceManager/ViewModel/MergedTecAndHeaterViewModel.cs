// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;

    using DeviceManager.View;

    public class MergedTecAndHeaterViewModel : IDisposable
    {
        // todo: make IDiposable and call dispose for child vm's
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
