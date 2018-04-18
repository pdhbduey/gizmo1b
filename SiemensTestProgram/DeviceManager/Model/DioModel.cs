using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public class DioModel : IDioModel
    {
        private IComCommunication communication;

        public DioModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<byte[]> SetDout(int channel, bool set)
        {
            var request = DioDefaults.SetDataOutByteArray(channel, set);
            communication.WriteData(request);
            return communication.ReadData();
        }

        public Task<byte[]> ReadDin()
        {
            var request = DioDefaults.ReadDioInCommand();
            communication.WriteData(request);
            return communication.ReadData();
        }
    }
}
