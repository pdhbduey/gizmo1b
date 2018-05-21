using Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager
{
    public static class TraceDefaults
    {
        public const int SampleNumberMinimum = 0;
        public const int SampleNumberMaximum = 1000;

        public static List<int> Resolutions = new List<int>
        {
            10,
            //100
        };

        public static byte[] SetStartCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0A,
                0x05,
                0x00,
                0x00,
                0x00,
                0x01
            };
        }

        public static byte[] SetStopCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0A,
                0x05,
                0x00,
                0x00,
                0x00,
                0x02
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
                0x06,
                0x00,
                0x00,
                0x00,
                0x01
            };
        }

        public static Dictionary<byte, string> StatusCodes = new Dictionary<byte, string>()
        {
            { 0x01,"Error resolution out of range" },
            { 0x02,"Error read samples out of range" }
        };

        public static byte[] ReadResolution()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0A,
                0x07,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetResolution(int resolution)
        {

            byte value;
            if (resolution == 10)
            {
                value = 0x00;
            }
            else 
            {
                value = 0x01;
            }

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0A,
                0x07,
                0x00,
                0x00,
                0x00,
                value
            };
        }

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

        public static byte[] SetReadNumberOfSamples(int samplesRead)
        {
            var value = Helper.ConvertIntToByteArray(samplesRead);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0A,
                0x0A,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] GetNumberOfAvailableSamples()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0A,
                0x09,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetFirstAvailableSamples()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0A,
                0x08,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }
    }
}
