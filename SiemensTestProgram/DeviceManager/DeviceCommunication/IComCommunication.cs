

using System.Collections.Generic;
using System.Threading.Tasks;

namespace DeviceManager.DeviceCommunication
{
    public interface IComCommunication
    {
        /// <summary>
        /// Sends data via the com port
        /// </summary>
        /// <param name="message"> Data to send. </param>
        void WriteData(byte[] request);

        Task<byte[]> ReadData();

        /// <summary>
        /// Updates configuration for serial communication to gizmo 1b device.
        /// </summary>
        /// <param name="comPort"> COM Port </param>
        /// <param name="baudRate"> Baud Rate </param>
        /// <param name="dataBits"> Data bits </param>
        /// <param name="parity"> Parity </param>
        /// <param name="stopBits"> Stop bits</param>
        void UpdateCommunication(string comPort, int baudRate, int dataBits, System.IO.Ports.Parity parity, System.IO.Ports.StopBits stopBits);

        List<string> GetPorts();

        int BaudRate { get; }

        string ComPort { get; }

        int DataBits { get; }

        System.IO.Ports.Parity Parity { get; }

        System.IO.Ports.StopBits StopBits { get; }

        void GetDeviceInformationAtPort(string selectedPort, ref string description, ref string name, ref string status, ref string pnpDeviceId);
    }
}
