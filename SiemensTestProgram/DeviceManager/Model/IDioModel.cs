using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IDioModel
    {
        Task<byte[]> SetDout(int channel, bool set);

        Task<byte[]> ReadDin();
    }
}
