// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Collections.Generic;
    using System.Threading.Tasks;

    /// <summary>
    /// Interface for LedModel class.
    /// </summary>
    public interface ILedModel
    {
        /// <summary>
        /// Writes data through the comp port.
        /// </summary>
        /// <param name="request"> Sends a request. </param>
        /// <returns> Task which returns byte array of response when completed. </returns>
        Task<byte[]> WriteData(byte[] request);

        ///// <summary>
        ///// Reconfigures the com port settings in the communication class.
        ///// </summary>
        ///// <param name="comPort"> Com port </param>
        ///// <param name="baudRate"> Baud rate </param>
        ///// <param name="dataBits"> Data bits </param>
        ///// <param name="parity"> Parity </param>
        ///// <param name="stopBits"> Stop bits </param>
        //void ReconfigureComCommunication(string comPort, int baudRate, int dataBits, System.IO.Ports.Parity parity, System.IO.Ports.StopBits stopBits);

        ///// <summary>
        ///// Gets the comp ports available.
        ///// </summary>
        ///// <returns> Available com ports. </returns>
        //List<string> GetPortSettings();

        ///// <summary>
        ///// Gets the default selected settings for serial communication.
        ///// </summary>
        ///// <param name="selectedComPort"> Comp port </param>
        ///// <param name="selectedBaudRate"> Baud rate </param>
        ///// <param name="dataBits"> Data bits </param>
        ///// <param name="selectedParity"> Selected parity </param>
        ///// <param name="selectedStopBits"> Stop bits </param>
        //void GetDefaultSettings(ref string selectedComPort, ref int selectedBaudRate, ref int dataBits, ref System.IO.Ports.Parity selectedParity, ref System.IO.Ports.StopBits selectedStopBits);
    }
}
