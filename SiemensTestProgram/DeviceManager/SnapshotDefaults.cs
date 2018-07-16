using Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager
{
    public static class SnapshotDefaults
    {
        public const int SampleNumberMinimum = 10;
        public const int SampleNumberMaximum = 1000;

        public static List<int> Resolutions = new List<int>
        {
            10,
            100,
            1000
        };

        public static byte[] StartSnapshot(bool raw)
        {
            byte value = 0x01;

            if (raw)
            {
                value = 0x05;
            }

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0A,
                0x00,
                0x00,
                0x00,
                0x00,
                value
            };
        }

        public static byte[] ReadPercentCompleted()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0A,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }


        public static byte[] StopSnapshot(bool raw)
        {
            byte value = 0x02;

            if (raw)
            {
                value = 0x06;
            }

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0A,
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
                0x0A,
                0x01,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static Dictionary<byte, string> StatusCodes = new Dictionary<byte, string>()
        {
            { 0x00, "Done" },
            { 0x01,"In Progress of snapshotting, retry." },
            { 0x02,"Sample out of range" },
            { 0x04,"Resolution out of range" },
            { 0x08,"Number of samples out of range" }
        };

        public static byte[] SetNumberOfSamples(int numberOfSamples)
        {
            var value = Helper.ConvertIntToByteArray(numberOfSamples);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0A,
                0x03,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] SetResolution(int resolution)
        {

            byte value;
            if (resolution == 10)
            {
                value = 0x00;
            }
            else if (resolution == 100)
            {
                value = 0x01;
            }
            else
            {
                value = 0x02;
            }

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0A,
                0x02,
                0x00,
                0x00,
                0x00,
                value
            };
        }

        public static byte[] ReadVsenseSamples(int sampleNumber)
        {
            var addressIncrement = Helper.ConvertIntToByteArray(sampleNumber);
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x10,
                0x00,
                addressIncrement[2],
                addressIncrement[3],
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadIsenseSamples(int sampleNumber)
        {
            var addressIncrement = Helper.ConvertIntToByteArray(sampleNumber);
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x20,
                0x00,
                addressIncrement[2],
                addressIncrement[3],
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadIrefSamples(int sampleNumber)
        {
            var addressIncrement = Helper.ConvertIntToByteArray(sampleNumber);
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x30,
                0x00,
                addressIncrement[2],
                addressIncrement[3],
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadTemperatureOneSamples(int sampleNumber)
        {
            var addressIncrement = Helper.ConvertIntToByteArray(sampleNumber);
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x40,
                0x00,
                addressIncrement[2],
                addressIncrement[3],
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadTemperatureTwoSamples(int sampleNumber)
        {
            var addressIncrement = Helper.ConvertIntToByteArray(sampleNumber);
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x50,
                0x00,
                addressIncrement[2],
                addressIncrement[3],
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadTemperatureThreeSamples(int sampleNumber)
        {
            var addressIncrement = Helper.ConvertIntToByteArray(sampleNumber);
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x60,
                0x00,
                addressIncrement[2],
                addressIncrement[3],
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadTemperatureFourSamples(int sampleNumber)
        {
            var addressIncrement = Helper.ConvertIntToByteArray(sampleNumber);
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x70,
                0x00,
                addressIncrement[2],
                addressIncrement[3],
                0x00,
                0x00,
                0x00,
                0x00
            };
        }
    }
}
