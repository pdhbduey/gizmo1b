using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IDacModel
    {
        /// <summary>
        /// Writes data through the comp port.
        /// </summary>
        /// <param name="request"> Sends a request. </param>
        /// <returns> Task which returns byte array of response when completed. </returns>
        Task<byte[]> WriteData(float voltage);
    }
}
