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

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects).
                    var heaterViewModel = PassedHeaterView as HeaterViewModel;
                    var tecViewModel = PassedTecView as TecViewModel;
                    var faultViewModel = PassedFaultView as FaultViewModel;

                    heaterViewModel.Dispose();
                    tecViewModel.Dispose();
                    faultViewModel.Dispose();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion
    }
}
