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
        bool SetDout(int channel, bool set, ref byte[] response);

        bool ReadDin(ref byte[] response);

        Task<CommunicationData> ReadDin();

        Task<CommunicationData> SetDout(int channel, bool set);
    }
}
