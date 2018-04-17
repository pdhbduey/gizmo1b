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
    }
}
