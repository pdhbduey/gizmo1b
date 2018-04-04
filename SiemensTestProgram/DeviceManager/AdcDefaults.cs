// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using Common;
    using System.Collections.Generic;

    public static class AdcDefaults
    {
        public static byte[] GetControlAdcCommand(int channel)
        {
            // Sets the start adc bit to 1
            var setChannel = Helper.ConvertIntToByteArray(channel);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x01,
                0x00,
                0x00,
                0x00,
                0x01,
                setChannel[3]
            };
        }

        public static byte[] GetStatusCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x01,
                0x01,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetReadAdcCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x01,
                0x02,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static Dictionary<byte, string> Errors = new Dictionary<byte, string>()
        {
            { 0x00, "Okay"},
            { 0x01, "Error invalid channel"},
            { 0x02, "Error timeout" }
        };
    }
}
