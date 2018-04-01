// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using Common;
    using System.Collections.Generic;

    public static class TecDefaults
    {
        public const int MaximumNumberOfSamples = 30000;
        public const int PeriodMinimum = 500;
        public const int PeriodMaximum = 10000;

        public const byte TEC_ENABLE = 0x01;
        public const byte TEC_DISABLE = 0x02;
        public const byte TEC_START_WAVEFORM = 0x04;
        public const byte TEC_STOP_WAVEFORM = 0x08;

        public static List<string> Waveforms = new List<string>
        {
            "Continuous",
            "Sine",
            "Square",
            "Triangle"
        };

        public static byte[] GetControlCommand(string command) 
        {
            var controlCommand = TecControlByteValues[command];
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x00,
                0x00,
                0x00,
                0x00,
                controlCommand
            }; 
        }

        public static byte[] GetIrefCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x07,
                0x01,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetIsenseCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x07,
                0x02,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetVsenseCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x07,
                0x03,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetStatusCommand()
        {      
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x07,
                0x04,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] GetWaveformTypeCommand(string waveform)
        {
            var waveformType = WaveFormByteValues[waveform];
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x05,
                0x00,
                0x00,
                0x00,
                waveformType
            };
        }

        public static byte[] GetWaveformPeriodCommand(int waveformPeriod)
        {
            var waveformPeriodArray = Helper.ConvertIntToByteArray(waveformPeriod);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x06,
                0x00,
                0x00,
                waveformPeriodArray[2],
                waveformPeriodArray[3]
            };
        }

        public static Dictionary<byte, string> Errors = new Dictionary<byte, string>()
        {
            { 0x00, "Okay"},
            { 0x01, "ISense Error"},
            { 0x02, "Error ref current out of range"},
            { 0x03, "Error set ref current"},
            { 0x04, "Error waveform type out of range"}
        };

        public static Dictionary<string, byte> WaveFormByteValues = new Dictionary<string, byte>()
        {
            { "Continuous", 0x00 },
            { "Sine", 0x01 },
            { "Square", 0x02 },
            { "Triangle", 0x03 }
        };

        public static Dictionary<string, byte> TecControlByteValues = new Dictionary<string, byte>()
        {
            { "Enable", TEC_ENABLE },
            { "Disable", TEC_DISABLE },
            { "Start Waveform", TEC_START_WAVEFORM },
            { "Stop Waveform", TEC_STOP_WAVEFORM }
        };
    }
}
