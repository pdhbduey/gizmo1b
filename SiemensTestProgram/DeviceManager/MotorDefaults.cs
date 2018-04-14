// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

using System.Collections.Generic;

namespace DeviceManager
{
    public static class MotorDefaults
    {
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

        public static byte[] SetStepModeCommand(string stepSize)
        {
            var value = StepSizeValues[stepSize];
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                MotorControlAddress[0],
                MotorControlAddress[1],
                MotorControlAddress[2],
                MotorControlAddress[3],
                0x00,
                0x00,
                value[0],
                value[1]
            };
        }

        public static byte[] SetDirectionCommand(string direction)
        {
            var value = DirectionValues[direction];
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                MotorControlAddress[0],
                MotorControlAddress[1],
                MotorControlAddress[2],
                MotorControlAddress[3],
                0x00,
                0x00,
                value,
                0x00
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
                0x40,
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

        public static byte[] MotorControlAddress = new byte[]
        {
            0x00,
            0x00,
            0x06,
            0x00
        };

        public static Dictionary<string, byte> DirectionValues = new Dictionary<string, byte>()
        {
            { Directions[0], 0x08},
            { Directions[1], 0x04}
        };

        public static Dictionary<string, byte[]> StepSizeValues = new Dictionary<string, byte[]>()
        {
            { StepSizes[0], GetStepModeValue(1)},
            { StepSizes[1], GetStepModeValue(2)},
            { StepSizes[2], GetStepModeValue(3)},
            { StepSizes[3], GetStepModeValue(4)},
            { StepSizes[4], GetStepModeValue(5)},
            { StepSizes[5], GetStepModeValue(6)},
            { StepSizes[6], GetStepModeValue(7)},
            { StepSizes[7], GetStepModeValue(8)}
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
