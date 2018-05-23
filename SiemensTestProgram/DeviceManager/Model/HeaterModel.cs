using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public class HeaterModel : IHeaterModel
    {
        private IComCommunication communication;

        public HeaterModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<CommunicationData> SetProportionalGainCommand(float gain)
        {
            var requestArray = HeaterDefaults.SetProportionalGain(gain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetIntegralGainCommand(float gain)
        {
            var requestArray = HeaterDefaults.SetIntegralGain(gain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetDerivativeGainCommand(float gain)
        {
            var requestArray = HeaterDefaults.SetDerivativeGain(gain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetTRefCommand(float tRef)
        {
            var requestArray = HeaterDefaults.SetTref(tRef);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetIMaxCommand(float iMax)
        {
            var requestArray = HeaterDefaults.SetImax(iMax);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
