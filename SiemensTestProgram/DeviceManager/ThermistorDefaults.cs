// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using Common;
    using System.Collections.Generic;

    public static class ThermistorDefaults
    {
        public static byte[] GetReadAinACommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x08,
                0x01,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetReadAinBCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x08,
                0x02,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetReadAinCCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x08,
                0x03,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetReadAinDCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x08,
                0x04,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetTypeCommand(string type)
        {
            var value = TypeMapping[type];

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x08,
                0x05,
                0x00,
                0x00,
                0x00,
                value
            };
        }

        public static byte[] GetStatusCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x08,
                0x00,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static List<string> Types = new List<string>
        {
            "USP12837",
            "SC30F103AN"
        };

        public static Dictionary<string, byte> TypeMapping = new Dictionary<string, byte>()
        {
            { Types[0], 0x01},
            { Types[1], 0x02},
        };

        public static Dictionary<byte, string> Errors = new Dictionary<byte, string>()
        {
            { 0x00, "Okay"},
            { 0x01, "Error invalid temp channel"},
            { 0x02, "Error read TEMP_AIN_A" },
            { 0x03, "Error read TEMP_AIN_B"},
            { 0x04, "Error read TEMP_AIN_C"},
            { 0x05, "Error read TEMP_AIN_D"}
        };
    }
}
