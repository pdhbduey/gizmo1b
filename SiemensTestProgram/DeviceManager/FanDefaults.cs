// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using Common;
    using System.Collections.Generic;

    public static class FanDefaults
    {
        public const int MinimumDutyCycle = 0;
        public const int MaximumDutyCycle = 100;
        public const int MinimumPeriod = 100;
        public const int MaximumPeriod = 1000000;
        public const int DefaultPeriod = 1000;

        public static byte[] GetStatusCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x03,
                0x04,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetFanSensorRpmCommand(int sensor)
        {
            byte sensorByte;

            sensorByte = 0x06;
            if (sensor == 1)
            {
                sensorByte = 0x05;
            }

            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x03,
                sensorByte,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetFanDutyCycleCommand(int channel, int dutyCycle)
        {
            var dutyCycleValue = Helper.ConvertIntToByteArray(dutyCycle);
            byte channelByte;

            channelByte = 0x01;
            if (channel == 1)
            {
                channelByte = 0x00;
            }

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x03,
                channelByte,
                dutyCycleValue[0],
                dutyCycleValue[1],
                dutyCycleValue[2],
                dutyCycleValue[3]
            };
        }

        public static byte[] GetFanDutyCycleCommand(int channel)
        {
            byte channelByte;

            channelByte = 0x01;
            if (channel == 1)
            {
                channelByte = 0x00;
            }

            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x03,
                channelByte,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetFanPeriodCommand(int channel, int period)
        {
            var periodValue = Helper.ConvertIntToByteArray(period);
            byte channelByte;

            channelByte = 0x03;
            if (channel == 1)
            {
                channelByte = 0x02;
            }

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x03,
                channelByte,
                periodValue[0],
                periodValue[1],
                periodValue[2],
                periodValue[3]
            };
        }

        public static byte[] GetFanPeriodCommand(int channel)
        {
            byte channelByte;

            channelByte = 0x03;
            if (channel == 1)
            {
                channelByte = 0x02;
            }

            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x03,
                channelByte,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static Dictionary<byte, string> Statuses = new Dictionary<byte, string>()
        {
            { 0x00, "Okay"},
            { 0x01, "Error duty cycle value out of range"},
            { 0x02, "Error period in us out of range" }
        };
    }
}
