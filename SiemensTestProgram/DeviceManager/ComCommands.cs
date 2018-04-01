// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using Common;

    public static class ComCommands
    {
        /// <summary>
        /// Types of commands.
        /// </summary>
        public enum CommandType
        {
            LED,
            DAC,
            TEC,
            Unknown
        }

        // Register read/write
        public const byte REGISTER_READ = 0x01;
        public const byte REGISTER_WRITE = 0x02;

        // Gets the array for LED command        
        public static byte[] GetLedCommand(byte rw, byte led)
        {
            return new byte[] 
            {
                rw,
                LED_ADDRESS[0],
                LED_ADDRESS[1],
                LED_ADDRESS[2],
                LED_ADDRESS[3],
                0x00,
                0x00,
                0x00,
                led
            };
        }

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

        // Gets the array for ADC control command
        public static byte[] GetAdcControlCommand(byte rw, int channel, int startAdc)
        {
            byte adcChannel = Helper.IntegerToByte(channel);
            byte startCommunication = Helper.IntegerToByte(startAdc);

            var adcCommand = new byte[9]
            {
                rw,
                0x00,
                0x00,
                0x01,
                0x00,
                0x00,
                0x00,
                startCommunication,
                adcChannel
            };

            return adcCommand;
        }

        // Gets the array for ADC status command
        public static byte[] GetAdcStatusCommand()
        {
            return new byte[]
            {
                REGISTER_READ,
                0x00,
                0x00,
                0x01,
                0x01,
                0x00,
                0x00,
                0x00,
                0x02
            };
        }

        // Gets the array for ADC result command
        public static byte[] GetAdcResultCommand()
        {
            return new byte[]
            {
                REGISTER_READ,
                0x00,
                0x00,
                0x01,
                0x02,
                0x00,
                0x00,
                0x00,
                0x02
            };
        }

        // LED ADDRESS
        public static byte[] LED_ADDRESS = new byte[4]
        {
            0x00,
            0x00,
            0x09,
            0x00
        };

        // SWITCH ADDRESS
        public static byte[] SWITCH_ADDRESS = new byte[4]
        {
            0x00,
            0x00,
            0x00,
            0x01
        };
    }
}
