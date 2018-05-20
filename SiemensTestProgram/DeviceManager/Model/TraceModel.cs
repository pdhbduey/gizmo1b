using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public class TraceModel : ITraceModel
    {
        private IComCommunication communication;

        public TraceModel(IComCommunication communication)
        {
            this.communication = communication;
        }
    }
}
