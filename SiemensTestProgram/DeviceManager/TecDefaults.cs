﻿// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using Common;
    using System.Collections.Generic;

    public static class TecDefaults
    {
        public const int MaximumNumberOfSamples = 30000;
        public const int PeriodMinimum = 2;
        public const int PeriodMaximum = 10000;
        public const int WaveformCyclesMinimum = 0;
        public const int WaveformCyclesMaximum = int.MaxValue;
        public const int SampleTimeMinimum = 0;
        public const int SampleTimeMaximum = 9999;
        //public const int IrefGainMinimum = 1;
        //public const int IrefGainMaximum = 3;

        public const float ProportionalGainMinimum = 0.01F;
        public const float ProportionalGainMaximum = 100;
        public const int DerivativeGainMinimum = 0;
        public const int DerivativeGainMaximum = 100;
        public const int IntegralGainMinimum = 0;
        public const int IntegralGainMaximum = 100;

        public const byte TEC_DISABLE = 0x01;
        public const byte TEC_ENABLE = 0x02;
        public const byte TEC_START_WAVEFORM = 0x04;
        public const byte TEC_STOP_WAVEFORM = 0x08;
        public const byte CLOSED_LOOP_DISABLE = 0x10;
        public const byte CLOSED_LOOP_ENABLE = 0x20;
        public const byte RESET_TEC_CUSTOM_WAVEFORM = 0x40;
        public const byte INCREMENT_TEC_CUSTOM_WAVEFORM = 0x80;

        internal const string StartCaptureText = "Start Capture";
        internal const string StopCaptureText = "Stop Capture";
        internal const string EnableText = "Enable";
        internal const string DisableText = "Disable";
        internal const string StartWaveformText = "Start Waveform";
        internal const string StopWaveformText = "Stop Waveform";
        internal const string EnableClosedLoopText = "Start Closed Loop";
        internal const string DisableClosedLoopText = "Stop Closed Loop";
        internal const string ResetTecWaveform = "ResetTecWaveform";
        internal const string IncrementTecWaveform = "IncrementTecWaveform";
        
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

        public static byte[] SetIrefCommand(float value)
        {
            var iref = Helper.GetBigEndian(value);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x0D,
                iref[0],
                iref[1],
                iref[2],
                iref[3]
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

        public static byte[] GetSampleTimeCommand(int sampleTime)
        {
            var sampleTimeValue = Helper.ConvertIntToByteArray(sampleTime);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x0C,
                sampleTimeValue[0],
                sampleTimeValue[1],
                sampleTimeValue[2],
                sampleTimeValue[3]
            };
        }

        public static byte[] GetWaveNumberOfCyclesCommand(int cycles)
        {
            var cycleValue = Helper.ConvertIntToByteArray(cycles);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x0E,
                cycleValue[0],
                cycleValue[1],
                cycleValue[2],
                cycleValue[3]
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
                waveformPeriodArray[0],
                waveformPeriodArray[1],
                waveformPeriodArray[2],
                waveformPeriodArray[3]
            };
        }

        public static byte[] GetDerivateGainCommand(int derivateGain)
        {
            var derivateGainArray = Helper.ConvertIntToByteArray(derivateGain);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x0A,
                derivateGainArray[0],
                derivateGainArray[1],
                derivateGainArray[2],
                derivateGainArray[3]
            };
        }

        public static byte[] GetIntegralGainCommand(int integralGain)
        {
            var integralGainArray = Helper.ConvertIntToByteArray(integralGain);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x09,
                integralGainArray[0],
                integralGainArray[1],
                integralGainArray[2],
                integralGainArray[3]
            };
        }

        public static byte[] GetProportionalGainCommand(float proportionalGain)
        {
            var proportionalGainArray = Helper.GetBigEndian(proportionalGain);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x07,
                0x08,
                proportionalGainArray[0],
                proportionalGainArray[1],
                proportionalGainArray[2],
                proportionalGainArray[3]
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
            { 0x0B, "Error custom waveform time not rising"},
            { 0x0C, "Error custom waveform non zero start time"}
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
            { DisableClosedLoopText, CLOSED_LOOP_DISABLE },
            { ResetTecWaveform, RESET_TEC_CUSTOM_WAVEFORM },
            { IncrementTecWaveform, INCREMENT_TEC_CUSTOM_WAVEFORM}
        };
    }
}
