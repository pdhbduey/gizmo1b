using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager
{
    using Common;

    public class DioDefaults
    {
        public const string notSetColour = "Gray";
        public const string setColour = "Green";

        public static byte[] ReadDioInCommand()
        {
            return new byte[]
            {
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x05,
                0x00,
                0x00,
                0x00,
                0x00,
                0x00
            };
        }

        public static byte[] SetDioOutCommand(int channel, bool set)
        {
            var value = SetDataOutByteArray(channel, set);
            return new byte[]
            {
                DataHelper.REGISTER_WRITE,
                0x00,
                0x00,
                0x05,
                0x01,
                value[0],
                value[1],
                value[2],
                value[3]
            };
        }

        private static byte[] SetDataOutByteArray(int channel, bool set)
        {
            var place = (2 * channel);
            if (!set)
            {
                place += 1;
            }

            var placeValue = (int)Math.Pow(2, place);
            return Helper.ConvertIntToByteArray(placeValue);
        }

        public static bool IsDinSet(byte[] channelData, int channel)
        {
            var channelBit = channel - (channel / 8);
            var position = channelData.Length - 1 - (channel / 8);
            return Helper.IsBitSet(channelData[position], channelBit);
        }
    }
}
