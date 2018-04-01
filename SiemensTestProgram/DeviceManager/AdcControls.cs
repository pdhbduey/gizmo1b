using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager
{
    public static class AdcControls
    {
        public static List<string> StartConversions = new List<string>()
        {
            "Off",
            "On"
        };

        public static List<int> AdcChannels = new List<int>()
        {
            0,
            1,
            2,
            3,
            4,
            5
        };
    }
}
