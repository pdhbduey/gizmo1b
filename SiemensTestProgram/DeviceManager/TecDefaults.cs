// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using Common;
    using System.Collections.Generic;

    public static class TecDefaults
    {
        public const int MaximumNumberOfSamples = 30000;
        public const int PeriodMinimum = 1;
        public const int PeriodMaximum = 10;
        public const int IrefGainMinimum = 1;
        public const int IrefGainMaximum = 3;

        public const byte TEC_DISABLE = 0x01;
        public const byte TEC_ENABLE = 0x02;
        public const byte TEC_START_WAVEFORM = 0x04;
        public const byte TEC_STOP_WAVEFORM = 0x08;
        public const byte CLOSED_LOOP_DISABLE = 0x10;
        public const byte CLOSED_LOOP_ENABLE = 0x20;

        internal const string StartCaptureText = "Start Capture";
        internal const string StopCaptureText = "Stop Capture";
        internal const string EnableText = "Enable";
        internal const string DisableText = "Disable";
        internal const string StartWaveformText = "Start Waveform";
        internal const string StopWaveformText = "Stop Waveform";
        internal const string EnableClosedLoopText = "Start Closed Loop";
        internal const string DisableClosedLoopText = "Stop Closed Loop";

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

        public static byte[] GetIrefGainCommand(int value)
        {
            var period = Helper.ConvertIntToByteArray(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x07,
                0x00,
                0x00,
                0x00,
                period[3]
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

        public static byte[] GetResetCommand()
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
                0x00,
                waveformPeriodArray[3]
            };
        }

        public static Dictionary<byte, string> Errors = new Dictionary<byte, string>()
        {
            { 0x00, "Okay"},
            { 0x01, "Error ISense"},
            { 0x02, "Error VSense" },
            { 0x03, "Error ref current out of range"},
            { 0x04, "Error set ref current"},
            { 0x05, "Error waveform type out of range"},
            { 0x06, "Error waveform period out of range"},
            { 0x07, "Error dac offset out of range"},
            { 0x08, "Error proportional gain out of range"},
            { 0x09, "Error integral gain out of range"},
            { 0x0A, "Error derivative gain out of range"},
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
            { EnableText, TEC_ENABLE },
            { DisableText, TEC_DISABLE },
            { StartWaveformText, TEC_START_WAVEFORM },
            { StopWaveformText, TEC_STOP_WAVEFORM },
            { EnableClosedLoopText, CLOSED_LOOP_ENABLE },
            { DisableClosedLoopText, CLOSED_LOOP_DISABLE }
        };
    }
}
