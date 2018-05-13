// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using DeviceManager.View;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Management;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace DeviceManager.DeviceCommunication
{
    /// <summary>
    /// Class handles communication protocol
    /// </summary>
    public class ComCommunication : IComCommunication, IDisposable
    {
        // Synchronization
        private volatile object mutex;
        private SemaphoreSlim requestSemaphore;
        private ErrorWindow errorDisplay;
        //private SemaphoreSlim readSemaphore;

        // Default configuration
        private const int readTimeout = 100;
        private const int readBufferSize = 5;
        private string comPort;
        private int baudRate = 115200;
        private int dataBits = 8;
        private Parity parity = System.IO.Ports.Parity.None;
        private StopBits stopBits = System.IO.Ports.StopBits.Two;
        private SerialPort serialPort;
        private bool isConfigured;

        public ComCommunication() 
        {
            isConfigured = false;
            mutex = new object();
            requestSemaphore = new SemaphoreSlim(1);
        }

        public int BaudRate => baudRate;

        public string ComPort => comPort;

        public int DataBits => dataBits;

        public Parity Parity => parity;

        public StopBits StopBits => stopBits;

        public bool IsConfigured => isConfigured;

        public Task<CommunicationData> ProcessCommunicationRequest(byte[] request)
        {
            return Task.Factory.StartNew(() =>
            {
                try
                {
                    if (!serialPort.IsOpen)
                    {
                        CreateSerialPort();
                    }

                    lock (mutex)
                    {
                        // Write to serial port
                        receivedData = false;
                        serialPort.DiscardOutBuffer();
                        serialPort.Write(request, 0, request.Length);
                        var timer = new Stopwatch();
                        timer.Start();

                        // Read from serial port
                        do
                        {
                            if (timer.Elapsed >= TimeSpan.FromMilliseconds(readTimeout))
                            {
                                throw new Exception($"Read operation exceeded timeout of {readTimeout} ms");
                            }
                        } while (!receivedData);

                        return new CommunicationData(true, dataBuffer);
                    }
                }
                catch
                {
                    if (serialPort != null && serialPort.IsOpen)
                    {
                        serialPort.Close();
                    }

                    CreateSerialPort();
                    return new CommunicationData(false, new byte[0]);
                }
            });
            
        }

        public bool ProcessCommunicationRequest(byte[] request, ref byte[] response)
        {
            try
            {
                if (!serialPort.IsOpen)
                {
                    CreateSerialPort();
                }

                lock (mutex)
                {
                    // Write to serial port
                    receivedData = false;
                    serialPort.DiscardOutBuffer();
                    serialPort.Write(request, 0, request.Length);
                    var timer = new Stopwatch();
                    timer.Start();

                    // Read from serial port
                    do
                    {
                        if (timer.Elapsed >= TimeSpan.FromMilliseconds(readTimeout))
                        {
                            throw new Exception($"Read operation exceeded timeout of {readTimeout} ms");
                        }
                    } while (!receivedData);

                    response = dataBuffer;
                }
            }
            catch
            {
                if (serialPort != null && serialPort.IsOpen)
                {
                    serialPort.Close();
                }

                CreateSerialPort();
                return false;
            }

            return true;
        }


        /// <summary>
        /// Updates configuration for serial communication to gizmo 1b device.
        /// </summary>
        /// <param name="comPort"> COM Port </param>
        /// <param name="baudRate"> Baud Rate </param>
        /// <param name="dataBits"> Data bits </param>
        /// <param name="parity"> Parity </param>
        /// <param name="stopBits"> Stop bits</param>
        public bool UpdateCommunication(string comPort, int baudRate, int dataBits, System.IO.Ports.Parity parity, System.IO.Ports.StopBits stopBits)
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

            return CreateSerialPort();
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

        private bool receivedData;
        private byte[] dataBuffer;

        private void HandleDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (serialPort.BytesToRead >= readBufferSize)
            {
                var buffer = new byte[5];
                serialPort.Read(buffer, 0, readBufferSize);
                dataBuffer = buffer;
                receivedData = true;
            }
        }

        private bool CreateSerialPort()
        {
            try
            {
                serialPort = new SerialPort(comPort, baudRate, parity, dataBits, stopBits);
                serialPort.ReadTimeout = readTimeout;
                serialPort.ReceivedBytesThreshold = readBufferSize;
                serialPort.DataReceived += HandleDataReceived;
                serialPort.Open();
                isConfigured = true;
            }
            catch
            {
                if (errorDisplay == null)
                {
                    errorDisplay = new ErrorWindow();
                    errorDisplay.Topmost = true;
                    errorDisplay.errorMsg.Content = "Error connecting to serial port.";
                    errorDisplay.Show();
                }
                
                isConfigured = false;
                return false;
            }

            return true;
        }
    }
}
