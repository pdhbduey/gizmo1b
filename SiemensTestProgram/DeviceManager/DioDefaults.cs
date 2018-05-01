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
                DataHelper.REGISTER_READ,
                0x00,
                0x00,
                0x05,
                0x01,
                value[3],
                value[2],
                value[1],
                value[0]
            };
        }

        public static byte[] SetDataOutByteArray(int channel, bool set)
        {
            var place = (2 * channel);
            if (!set)
            {
                place += 1;
            }

            var placeValue = (int)Math.Pow(2, place);
            return Helper.ConvertIntToByteArray(placeValue);
        }

        public static bool IsDinSet(byte[] channelData, int channel, bool set)
        {
            var channelIndex = (2 * channel);
            if (!set)
            {
                channelIndex += 1;
            }

            var position = channelIndex / 4;
            var channelBit = position % 4;
            return Helper.IsBitSet(channelData[position], channelBit);
        }
    }
}
