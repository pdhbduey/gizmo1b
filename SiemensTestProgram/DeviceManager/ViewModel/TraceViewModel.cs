using Common.Bindings;
using DeviceManager.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.ViewModel
{
    public class TraceViewModel : BindableBase
    {
        private ITraceModel traceModel;

        public TraceViewModel(ITraceModel traceModel)
        {
            this.traceModel = traceModel;
        }
    }
}
