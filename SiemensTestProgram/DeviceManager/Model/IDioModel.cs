using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IDioModel
    {
        Task<CommunicationData> ReadDout();

        Task<CommunicationData> ReadDin();

        Task<CommunicationData> SetDout(int channel, bool set);
    }
}
