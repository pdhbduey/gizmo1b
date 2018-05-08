using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface ICommunicationConfigurationModel
    {
        /// <summary>
        /// Reconfigures the com port settings in the communication class.
        /// </summary>
        /// <param name="comPort"> Com port </param>
        /// <param name="baudRate"> Baud rate </param>
        /// <param name="dataBits"> Data bits </param>
        /// <param name="parity"> Parity </param>
        /// <param name="stopBits"> Stop bits </param>
        string ReconfigureComCommunication(string comPort, int baudRate, int dataBits, System.IO.Ports.Parity parity, System.IO.Ports.StopBits stopBits);

        /// <summary>
        /// Gets the comp ports available.
        /// </summary>
        /// <returns> Available com ports. </returns>
        List<string> GetPortSettings();

        /// <summary>
        /// Gets the default selected settings for serial communication.
        /// </summary>
        /// <param name="selectedComPort"> Comp port </param>
        /// <param name="selectedBaudRate"> Baud rate </param>
        /// <param name="dataBits"> Data bits </param>
        /// <param name="selectedParity"> Selected parity </param>
        /// <param name="selectedStopBits"> Stop bits </param>
        void GetDefaultSettings(ref string selectedComPort, ref int selectedBaudRate, ref int dataBits, ref System.IO.Ports.Parity selectedParity, ref System.IO.Ports.StopBits selectedStopBits);

        void GetDeviceInformationAtPort(string selectedPort, ref string description, ref string name, ref string status, ref string pnpDeviceId);
    }
}
