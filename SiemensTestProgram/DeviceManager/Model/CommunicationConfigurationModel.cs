// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using DeviceManager.DeviceCommunication;
using System.Collections.Generic;

namespace DeviceManager.Model
{
    public class CommunicationConfigurationModel : ICommunicationConfigurationModel
    {
        private IComCommunication communication;

        public CommunicationConfigurationModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        /// <summary>
        /// Reconfigures the com port settings in the communication class.
        /// </summary>
        /// <param name="comPort"> Com port </param>
        /// <param name="baudRate"> Baud rate </param>
        /// <param name="dataBits"> Data bits </param>
        /// <param name="parity"> Parity </param>
        /// <param name="stopBits"> Stop bits </param>
        public string ReconfigureComCommunication(string comPort, int baudRate, int dataBits, System.IO.Ports.Parity parity, System.IO.Ports.StopBits stopBits)
        {
            var configured = communication.UpdateCommunication(comPort, baudRate, dataBits, parity, stopBits);

            if (configured)
            {
                return "COM Port succesfully opened.";
            }

            return "Error configuring COM Port";
        }

        /// <summary>
        /// Gets the comp ports available.
        /// </summary>
        /// <returns> Available com ports. </returns>
        public List<string> GetPortSettings()
        {
            return communication.GetPorts();
        }

        /// <summary>
        /// Gets the default selected settings for serial communication.
        /// </summary>
        /// <param name="selectedComPort"> Comp port </param>
        /// <param name="selectedBaudRate"> Baud rate </param>
        /// <param name="dataBits"> Data bits </param>
        /// <param name="selectedParity"> Selected parity </param>
        /// <param name="selectedStopBits"> Stop bits </param>
        public void GetDefaultSettings(ref string selectedComPort, ref int selectedBaudRate, ref int dataBits, ref System.IO.Ports.Parity selectedParity, ref System.IO.Ports.StopBits selectedStopBits)
        {
            selectedComPort = communication.ComPort;
            selectedBaudRate = communication.BaudRate;
            dataBits = communication.DataBits;
            selectedParity = communication.Parity;
            selectedStopBits = communication.StopBits;
        }

        public void GetDeviceInformationAtPort(string selectedPort, ref string description, ref string name, ref string status, ref string pnpDeviceId)
        {
            communication.GetDeviceInformationAtPort(selectedPort, ref description, ref name, ref status, ref pnpDeviceId);
        }
    }
}
