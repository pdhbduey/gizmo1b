﻿using Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager
{
    public static class HeaterDefaults
    {
        public const int WaveformCyclesMinimum = 0;
        public const int WaveformCyclesMaximum = int.MaxValue;
        public const float ProportionalGainMinimum = 0.01F;
        public const float ProportionalGainMaximum = 100;
        public const float DerivativeGainMinimum = 0;
        public const float DerivativeGainMaximum = 100;
        public const float IntegralGainMinimum = 0;
        public const float IntegralGainMaximum = 100;
        public const float TRefMinimum = 0;
        public const float TRefMaximum = 100;
        public const float IMaxMinimum = 0;
        public const float IMaxMaximum = 15;

        public static List<string> Tins = new List<string>
        {
            "T1",
            "T2",
            "T3",
            "T4"
        };

        public static Dictionary<byte, string> TinReadMapping = new Dictionary<byte, string>()
        {
            { 0x00, "T1" },
            { 0x01, "T2" },
            { 0x02, "T3" },
            { 0x03, "T4" }
        };

        public static byte[] ReadSelect()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x04,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadCycles()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadControl()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00,
                0x01
            };
        }

        public static byte[] SetDisableCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00,
                0x01
            };
        }

        public static byte[] SetEnableCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00,
                0x02
            };
        }

        public static byte[] SetDisableClosedLoopCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00,
                0x10
            };
        }

        public static byte[] SetEnableClosedLoopCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00,
                0x20
            };
        }

        public static byte[] ResetCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00,
                0x40
            };
        }

        public static byte[] IncrementCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00,
                0x80
            };
        }

        
        public static byte[] ReadWaveformIndexCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x08,
                0x00,
                0x00,
                0x00,
                0x80
            };
        }

        public static byte[] ReadStatus()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x01,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] ReadTref()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x02,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetTref(float value)
        {
            var valueBytes = Helper.GetBigEndian(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x02,
                valueBytes[0],
                valueBytes[1],
                valueBytes[2],
                valueBytes[3]
            };
        }

        public static byte[] SetCustomWaveformTref(float value)
        {
            var valueBytes = Helper.GetBigEndian(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x0A,
                valueBytes[0],
                valueBytes[1],
                valueBytes[2],
                valueBytes[3]
            };
        }

        public static byte[] SetCustomWaveformTime(int value)
        {
            var valueBytes = Helper.ConvertIntToByteArray(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x09,
                valueBytes[0],
                valueBytes[1],
                valueBytes[2],
                valueBytes[3]
            };
        }

        public static byte[] ReadImax()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x03,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetImax(float value)
        {
            var valueBytes = Helper.GetBigEndian(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x03,
                valueBytes[0],
                valueBytes[1],
                valueBytes[2],
                valueBytes[3]
            };
        }

        public static byte[] ReadProportionalGain()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x05,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetWaveformControl(string state)
        {
            byte value = 0x04;

            if (state.Equals(TecDefaults.StopWaveformText))
            {
                value = 0x08;
            }

            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x00,
                0x00,
                0x00,
                0x00,
                value
            };
        }

        public static byte[] SetWaveformCycles(int value)
         {
            var valueBytes = Helper.ConvertIntToByteArray(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x0B,
                valueBytes[0],
                valueBytes[1],
                valueBytes[2],
                valueBytes[3]
            };
        }

        public static byte[] SetProportionalGain(float value)
        {
            var valueBytes = Helper.GetBigEndian(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x05,
                valueBytes[0],
                valueBytes[1],
                valueBytes[2],
                valueBytes[3]
            };
        }

        public static byte[] ReadIntegralGain()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x06,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetIntegralGain(float value)
        {
            var valueBytes = Helper.GetBigEndian(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x06,
                valueBytes[0],
                valueBytes[1],
                valueBytes[2],
                valueBytes[3]
            };
        }

        public static byte[] ReadDerivativeGain()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x07,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetDerivativeGain(float value)
        {
            var valueBytes = Helper.GetBigEndian(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x07,
                valueBytes[0],
                valueBytes[1],
                valueBytes[2],
                valueBytes[3]
            };
        }

        public static Dictionary<byte, int> TinValues = new Dictionary<byte, int>()
        {
            { 0x00, 1 },
            { 0x01, 2 },
            { 0x02, 3 },
            { 0x03, 4 }
        };

        public static Dictionary<byte, string> StatusValues = new Dictionary<byte, string>()
        {
            { 0x00, "Okay" },
            { 0x01, "Error Tref value out of range" },
            { 0x02, "Error Imax value out of range" },
            { 0x03, "Error proportional gain value out of range" },
            { 0x04, "Error integral gain value out of range" },
            { 0x05, "Error derivative gain value out of range" },
            { 0x06, "Error Tin select out of range" }
        };

        public static byte[] ReadTinSelect()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x0B,
                0x04,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetTinSelect(int tin)
        {
            var value = Helper.ConvertIntToByteArray(tin);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x0B,
                0x04,
                0x00,
                0x00,
                0x00,
                value[3]
            };
        }
    }
}
