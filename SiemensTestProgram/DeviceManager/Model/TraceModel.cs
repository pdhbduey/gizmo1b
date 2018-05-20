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

        public Task<CommunicationData> StartTrace()
        {
            var requestArray = TraceDefaults.SetStartCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> StopTrace()
        {
            var requestArray = TraceDefaults.SetStopCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetResolution(int resolution)
        {
            var requestArray = TraceDefaults.SetResolution(resolution);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetNumberOfSamples(int sampleNumber)
        {
            var requestArray = TraceDefaults.SetNumberOfSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
