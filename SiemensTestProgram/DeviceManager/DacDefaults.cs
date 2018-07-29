// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using System.Collections.Generic;

    public static class DacDefaults
    {
        // Gets the array for setting DAC command.   
        public static byte[] SetDacCommand(byte[] value)
        {
            if (value.Length != 4)
            {
                return null;
            }

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                DacValueAddress[0],
                DacValueAddress[1],
                DacValueAddress[2],
                DacValueAddress[3],
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] ReadDacValueCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                DacValueAddress[0],
                DacValueAddress[1],
                DacValueAddress[2],
                DacValueAddress[3],
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        // Gets the array for reading DAC command.   
        public static byte[] ReadDacCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x02,
                0x01,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        /// <summary>
        /// DAC status codes.
        /// </summary>
        public static Dictionary<byte, string> DacStatus = new Dictionary<byte, string>()
        {
            { 0x00, "Okay"},
            { 0x01, "Set value out of range"},
            { 0x02, "Timeout error" }
        };

        /// <summary>
        /// Address for DAC value register.
        /// </summary>
        private static byte[] DacValueAddress = new byte[]
        {
            0x00,
            0x00,
            0x02,
            0x00
        };
    }
}
