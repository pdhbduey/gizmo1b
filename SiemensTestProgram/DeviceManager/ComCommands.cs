// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    public static class ComCommands
    {
        // Register read/write
        public const byte REGISTER_READ = 0x01;
        public const byte REGISTER_WRITE = 0x02;

        
        // Gets the array for DAC command   
        public static byte[] GetDacCommand(byte rw, byte[] value)
        {
            if (value.Length != 4)
            {
                return null;
            }

            return new byte[]
            {
                rw,
                0x00,
                0x00,
                0x02,
                0x00,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }
    }
}
