// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using System.Collections.Generic;

    public static class LedDefaults
    {
        public const string redLedOn = "LED Red On";
        public const string redLedOff = "LED Red Off";
        public const string greenLedOn = "LED Green On";
        public const string greenLedOff = "LED Green Off";

        // Gets the array for LED write command        
        public static byte[] GetLedWriteCommand(string ledCommand)
        {
            var led = LedSetValue[ledCommand];
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x09,
                0x00,
                0x00,
                0x00,
                0x00,
                led
            };
        }

        // Gets the array for LED read command        
        public static byte[] GetLedReadCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x09,
                0x00,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        // Mapping for bytes to and LED status
        public static Dictionary<string, byte> LedSetValue = new Dictionary<string, byte>()
        {
            { redLedOn, 0x01 },
            { redLedOff, 0x02 },
            { greenLedOn, 0x04 },
            { greenLedOff, 0x08 }
        };


        // Led Status values
        public static Dictionary<byte, string> LedStatus = new Dictionary<byte, string>()
        {
            { 0x01, "LED Red On"},
            { 0x02, "LED Red Off"},
            { 0x04, "LED Green On"},
            { 0x08, "LED Green Off"}
        };

    }
}
