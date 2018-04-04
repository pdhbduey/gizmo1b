// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Management;
using System.Threading;
using System.Threading.Tasks;

namespace DeviceManager.DeviceCommunication
{
    /// <summary>
    /// Class handles communication protocol
    /// </summary>
    public class ComCommunication : IComCommunication, IDisposable
    {
        // Synchronization
        private volatile object mutex = new object();
        private SemaphoreSlim requestSemaphore;
        //private SemaphoreSlim readSemaphore;

        // Default configuration
        private const int timeout = 10000;
        private const int byteThreshold = 5;
        private string comPort = "COM9";
        private int baudRate = 115200;
        private int dataBits = 8;
        private Parity parity = System.IO.Ports.Parity.None;
        private StopBits stopBits = System.IO.Ports.StopBits.Two;
        private SerialPort serialPort;
        private bool isConfigured;

        public ComCommunication() 
        {
            isConfigured = false;
            requestSemaphore = new SemaphoreSlim(1);
            //readSemaphore = new SemaphoreSlim(1);
            CreateSerialPort();
        }

        public int BaudRate => baudRate;

        public string ComPort => comPort;

        public int DataBits => dataBits;

        public Parity Parity => parity;

        public StopBits StopBits => stopBits;

        public bool IsConfigured => isConfigured;

        /// <summary>
        /// Sends data via the com port
        /// </summary>
        /// <param name="message"> Data to send. </param>
        public void WriteData(byte[] request)
        {
            try
            {
                if (serialPort.IsOpen)
                {
                    requestSemaphore.Wait(1);
                    serialPort.DiscardOutBuffer();
                    serialPort.Write(request, 0, request.Length);
                }
            }
            catch
            {
                serialPort.DiscardInBuffer();
                serialPort.DiscardOutBuffer();
            }
        }

        public Task<byte[]> ReadData()
        {
            return Task.Factory.StartNew(() =>
            {
                var time = 0;
                var data = new byte[5];
                try
                {
                    if (serialPort.IsOpen)
                    {
                        do
                        {
                            time += 10;
                            if (time > timeout) return data;
                            Thread.Sleep(10);
                        } while (serialPort.BytesToRead < 5);

                        serialPort.Read(data, 0, data.Length);
                        requestSemaphore.Release(1);
                    }
                }
                catch (Exception e)
                {
                   return new byte[0];
                }
                    
                return data;
            });
            
        }

        /// <summary>
        /// Updates configuration for serial communication to gizmo 1b device.
        /// </summary>
        /// <param name="comPort"> COM Port </param>
        /// <param name="baudRate"> Baud Rate </param>
        /// <param name="dataBits"> Data bits </param>
        /// <param name="parity"> Parity </param>
        /// <param name="stopBits"> Stop bits</param>
        public void UpdateCommunication(string comPort, int baudRate, int dataBits, System.IO.Ports.Parity parity, System.IO.Ports.StopBits stopBits)
        {
            this.comPort = comPort;
            this.baudRate = baudRate;
            this.dataBits = dataBits;
            this.parity = parity;
            this.stopBits = stopBits;

            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.Close();
            }

            CreateSerialPort();
        }

        public List<string> GetPorts()
        {
            var portsList = new List<string>();
            foreach (var port in SerialPort.GetPortNames())
            {
                portsList.Add(port);
            }

            return portsList;
        }

        public void GetDeviceInformationAtPort(string selectedPort, ref string description, ref string name, ref string status, ref string pnpDeviceId)
        {
            description = string.Empty;
            name = string.Empty;
            status = string.Empty;
            pnpDeviceId = string.Empty;

            ManagementObjectCollection mbsList = null;
            ManagementObjectSearcher mbs = new ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_SerialPort");
            mbsList = mbs.Get();

            foreach (ManagementObject mo in mbsList)
            {
                var comPort = mo["DeviceId"].ToString();

                if (comPort == selectedPort)
                {
                    description = mo["Description"].ToString();
                    name = mo["Name"].ToString();
                    status = mo["Status"].ToString();
                    pnpDeviceId = mo["PNPDeviceID"].ToString();
                }
            }
        }

        public void Dispose()
        {
            if (serialPort != null & serialPort.IsOpen)
            {
                serialPort.Close();
            }
        }

        private void CreateSerialPort()
        {
            try
            {
                serialPort = new SerialPort(comPort, baudRate, parity, dataBits, stopBits);
                serialPort.Open();
                serialPort.ReadTimeout = timeout;
                serialPort.ReceivedBytesThreshold = byteThreshold;
            }
            catch
            {

            }
        }
    }
}
