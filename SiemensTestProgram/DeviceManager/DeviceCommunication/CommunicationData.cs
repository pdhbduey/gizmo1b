using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.DeviceCommunication
{
    public class CommunicationData
    {
        public bool succesfulResponse;
        public byte[] response;
        

        public CommunicationData(bool succesfulResponse, byte[] response)
        {
            this.response = response;
            this.succesfulResponse = succesfulResponse;
        }
    }
}
