// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    public static class FaultDefaults
    {
        public static byte[] ResetCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x04,
                0x00,
                0x00,
                0x00,
                0x00,
                0x01
            };
        }

        public static byte[] ReadStateCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x04,
                0x01,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadNtcCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x04,
                0x02,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }
    }
}
