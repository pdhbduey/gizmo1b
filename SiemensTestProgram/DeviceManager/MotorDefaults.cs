// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using Common;
using System.Collections.Generic;

namespace DeviceManager
{
    public static class MotorDefaults
    {
        public const int RelativePositionMinimum = 0;
        public const int RelativePositionMaximum = 4194303;
        public const int AbsolutePositionMinimum = -2197152;
        public const int AbsolutePositionMaximum = 2197153;

        public static List<string> StepSizes = new List<string>
        {
            "Full Step",
            "Half Step",
            "1/4 Micro Step",
            "1/8 Micro Step",
            "1/16 Micro Step",
            "1/32 Micro Step",
            "1/64 Micro Step",
            "1/128 Micro Step",
        };

        public static List<string> Directions = new List<string>
        {
            "Forward",
            "Reverse"
        };

        public static byte[] SetInitializeCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                MotorControlAddress[0],
                MotorControlAddress[1],
                MotorControlAddress[2],
                MotorControlAddress[3],
                0x00,
                0x00,
                0x00,
                0x01
            };
        }

        public static byte[] SetControlMoveCommand(string direction, string stepMode, string move)
        {
            var directionValue = DirectionValues[direction];
            var stepModeValue = StepSizeValues[stepMode];
            var moveValue = MoveValues[move];
            var value = Helper.ConvertIntToByteArray(directionValue + stepModeValue + moveValue);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x06,
                0x00,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] CycleCommand()
        {
            var moveValue = MoveValues["cycle"];
            var value = Helper.ConvertIntToByteArray(moveValue);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x06,
                0x00,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] StopCommand()
        {
            var moveValue = MoveValues["stop"];
            var value = Helper.ConvertIntToByteArray(moveValue);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x06,
                0x00,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] ReadPositionCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x06,
                0x07,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetMoveAbsolutePositionCommand(int absolutePosition)
        {
            var value = Helper.ConvertIntToByteArray(absolutePosition);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x06,
                0x03,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] SetMoveRelativePositionCommand(int relativePosition)
        {
            var value = Helper.ConvertIntToByteArray(relativePosition);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x06,
                0x02,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        public static byte[] SetMoveRelativeCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                MotorControlAddress[0],
                MotorControlAddress[1],
                MotorControlAddress[2],
                MotorControlAddress[3],
                0x00,
                0x00,
                0x00,
                0x02
            };
        }

        public static byte[] SetMoveAbsoluteCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                MotorControlAddress[0],
                MotorControlAddress[1],
                MotorControlAddress[2],
                MotorControlAddress[3],
                0x00,
                0x00,
                0x00,
                0x04
            };
        }

        public static byte[] SetLimpCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                MotorControlAddress[0],
                MotorControlAddress[1],
                MotorControlAddress[2],
                MotorControlAddress[3],
                0x00,
                0x00,
                0x00,
                0x08
            };
        }

        public static byte[] SetResetCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                MotorControlAddress[0],
                MotorControlAddress[1],
                MotorControlAddress[2],
                MotorControlAddress[3],
                0x00,
                0x00,
                0x20,
                0x00
            };
        }

        public static byte[] SetEnergizeCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                MotorControlAddress[0],
                MotorControlAddress[1],
                MotorControlAddress[2],
                MotorControlAddress[3],
                0x00,
                0x00,
                0x00,
                0x0A
            };
        }

        public static byte[] SetHomeCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                MotorControlAddress[0],
                MotorControlAddress[1],
                MotorControlAddress[2],
                MotorControlAddress[3],
                0x00,
                0x00,
                0x00,
                0x0C
            };
        }

        public static byte[] ReadRegisterValueCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x06,
                0x0A,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetRegisterValueCommand(byte[] value)
        {
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x06,
                0x0A,
                value[3],
                value[2],
                value[1],
                value[0],
            };
        }

        public static byte[] SetRegisterAddressCommand(string address)
        {
            var addressValue = RegisterAddressValues[address];
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x06,
                0x0A,
                0x00,
                0x00,
                0x00,
                addressValue,
            };
        }

        public static byte[] MotorControlAddress = new byte[]
        {
            0x00,
            0x00,
            0x06,
            0x00
        };

        public static List<string> RegisterAddresses = new List<string>
        {
            "ABS_POS",
            "EL_POS",
            "MARK",
            "SPEED",
            "ACC",
            "DEC",
            "MAX_SPEED",
            "MIN_SPEED",
            "FS_SPD",
            "KVAL_HOLD",
            "KVAL_RUN",
            "KVAL_ACC",
            "KVAL_DEC",
            "INT_SPEED",
            "ST_SLP",
            "FN_SLP_ACC",
            "FN_SLP_DEC",
            "K_THERM",
            "ADC_OUT",
            "OCD_TH",
            "STALL_TH",
            "STEP_MODE",
            "ALARM_EN",
            "CONFIG",
            "STATUS"
        };

        public static Dictionary<string, int> DirectionValues = new Dictionary<string, int>()
        {
            { Directions[0], 1024},
            { Directions[1], 512}
        };

        public static Dictionary<string, int> MoveValues = new Dictionary<string, int>()
        {
            { "relative", 2},
            { "absolute", 4},
            { "cycle", 6},
            { "stop", 12}
        };

        public static Dictionary<string, int> StepSizeValues = new Dictionary<string, int>()
        {
            { StepSizes[0], 32},
            { StepSizes[1], 64},
            { StepSizes[2], 96},
            { StepSizes[3], 128},
            { StepSizes[4], 160},
            { StepSizes[5], 192},
            { StepSizes[6], 224},
            { StepSizes[7], 256}
        };

        public static Dictionary<string, byte> RegisterAddressValues = new Dictionary<string, byte>()
        {
            {RegisterAddresses[0], 0x01 },
            {RegisterAddresses[1], 0x02 },
            {RegisterAddresses[2], 0x03 },
            {RegisterAddresses[3], 0x04 },
            {RegisterAddresses[4], 0x05 },
            {RegisterAddresses[5], 0x06 },
            {RegisterAddresses[6], 0x07 },
            {RegisterAddresses[7], 0x08 },
            {RegisterAddresses[8], 0x15 },
            {RegisterAddresses[9], 0x09 },
            {RegisterAddresses[10], 0x0a },
            {RegisterAddresses[11], 0x0b },
            {RegisterAddresses[12], 0x0c },
            {RegisterAddresses[13], 0x0d },
            {RegisterAddresses[14], 0x0e },
            {RegisterAddresses[15], 0x0f },
            {RegisterAddresses[16], 0x10 },
            {RegisterAddresses[17], 0x11 },
            {RegisterAddresses[18], 0x12 },
            {RegisterAddresses[19], 0x13 },
            {RegisterAddresses[20], 0x14 },
            {RegisterAddresses[21], 0x16 },
            {RegisterAddresses[22], 0x17 },
            {RegisterAddresses[23], 0x18 },
            {RegisterAddresses[24], 0x19 },
        };
             
        public static byte[] GetStepModeValue(int mode)
        {
            var value = new byte[2];
            switch(mode)
            {
                case 1:
                    value[0] = 0x00;
                    value[1] = 0x20;
                    break;
                case 2:
                    value[0] = 0x00;
                    value[1] = 0x40;
                    break;
                case 3:
                    value[0] = 0x00;
                    value[1] = 0x60;
                    break;
                case 4:
                    value[0] = 0x00;
                    value[1] = 0x80;
                    break;
                case 5:
                    value[0] = 0x00;
                    value[1] = 0xA0;
                    break;
                case 6:
                    value[0] = 0x00;
                    value[1] = 0xC0;
                    break;
                case 7:
                    value[0] = 0x00;
                    value[1] = 0xE0;
                    break;
                case 8:
                    value[0] = 0x01;
                    value[1] = 0x00;
                    break;
            }

            return value;
        }
    }
}
