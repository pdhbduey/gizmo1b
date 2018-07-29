using Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager
{
    public static class OpticsDefault
    {
        public const int IntegrationTimeMinimum = 1000;
        public const int IntegrationTimeDefault = 10000;
        public const int IntegrationTimeMaximum = 1000000;
        public const int IntensityMinimum = 0;
        public const int IntensityMaximum = 40000;

        public static List<string> Leds = new List<string>
        {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6"
        };

        public static List<int> LedBoardVersions = new List<int>
        {
            1,
            2
        };

        public static List<int> PdBoardVersions = new List<int>
        {
            1,
            2
        };

        public static Dictionary<string, byte> SelectLedMapping = new Dictionary<string, byte>()
        {
            { Leds[0], 0x01},
            { Leds[1], 0x02},
            { Leds[2], 0x03},
            { Leds[3], 0x04},
            { Leds[4], 0x05},
            { Leds[5], 0x06},
        };

        public static List<string> Photodiodes = new List<string>
        {
            "1",
            "2",
            "3",
            "4",
            "5",
            "6"
        };

        public static Dictionary<string, byte> SelectPhotodiodeMapping = new Dictionary<string, byte>()
        {
            { Photodiodes[0], 0x08},
            { Photodiodes[1], 0x10},
            { Photodiodes[2], 0x18},
            { Photodiodes[3], 0x20},
            { Photodiodes[4], 0x28},
            { Photodiodes[5], 0x30},
        };

        public static byte[] SetLedControlCommand(string led)
        {
            var value = SelectLedMapping[led];

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x00,
                0x00,
                0x00,
                0x00,
                value
            };
        }

        public static byte[] EnableLedBoardCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x00,
                0x00,
                0x00,
                0x00,
                0x40
            };
        }

        public static byte[] DisableLedBoardCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x00,
                0x00,
                0x00,
                0x00,
                0x80
            };
        }

        public static byte[] EnablePdBoardCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x00,
                0x00,
                0x00,
                0x01,
                0x00
            };
        }

        public static byte[] DisablePdBoardCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x00,
                0x00,
                0x00,
                0x02,
                0x00
            };
        }

        public static byte[] ResetControlSettingsCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x00,
                0x00,
                0x00,
                0x01,
                0x49
            };
        }

        public static byte[] SetPhotodiodeControlCommand(string photodiode)
        {
            var value = SelectPhotodiodeMapping[photodiode];

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x00,
                0x00,
                0x00,
                0x00,
                value
            };
        }

        public static byte[] ReadStatus()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0C,
                0x01,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetLedBoardVersionCommand(int version)
        {
            byte value = Helper.IntegerToByte(version);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x06,
                0x00,
                0x00,
                0x00,
                value
            };
        }

        public static byte[] SetPdBoardVersionCommand(int version)
        {
            byte value = Helper.IntegerToByte(version);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x07,
                0x00,
                0x00,
                0x00,
                value
            };
        }

        public static byte[] ReadLedTemperatureDuringIntegrationCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0C,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadPdTemperatureDuringIntegrationCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0C,
                0x0C,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadLedTemperatureCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0C,
                0x08,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadPdTemperatureCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0C,
                0x09,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadLedMonitorVolts()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0C,
                0x0A,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static Dictionary<byte, string> StatusValues = new Dictionary<byte, string>()
        {
            { 0x00, "Okay" },
            { 0x01, "Error integration time out of range" },
            { 0x02, "Error error select LED out of range" },
            { 0x03, "Error select photodiode out of range" },
            { 0x04, "Error error led intensity out of range" }
        };

        public static byte[] SetIntegrationTime(int integrationTime)
        {
            var value = Helper.ConvertIntToByteArray(integrationTime);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x02,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] SetIntensity(int itensity)
        {
            var value = Helper.ConvertIntToByteArray(itensity);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0C,
                0x03,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] ReadPhotodiodeVolts()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0C,
                0x04,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadPhotodiodeRaw()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0C,
                0x05,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }
    }
}
