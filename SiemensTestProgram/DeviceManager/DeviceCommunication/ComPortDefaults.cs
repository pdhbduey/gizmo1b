using System.Collections.Generic;

namespace DeviceManager.DeviceCommunication
{
    internal static class ComPortDefaults
    {
        public static List<int> BaudRates = new List<int>()
        {
            19200,
            115200,
            128000
        };

        public static List<System.IO.Ports.Parity> Parities = new List<System.IO.Ports.Parity>()
        {
            System.IO.Ports.Parity.Even,
            System.IO.Ports.Parity.Odd,
            System.IO.Ports.Parity.None,
            System.IO.Ports.Parity.Space,
            System.IO.Ports.Parity.Mark,
        };

        public static List<System.IO.Ports.StopBits> StopBits = new List<System.IO.Ports.StopBits>()
        {
            System.IO.Ports.StopBits.None,
            System.IO.Ports.StopBits.One,
            System.IO.Ports.StopBits.OnePointFive,
            System.IO.Ports.StopBits.Two
        };
    }
}
