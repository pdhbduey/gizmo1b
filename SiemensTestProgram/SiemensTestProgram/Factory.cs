using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SiemensTestProgram
{
    public sealed class Factory
    {
        private static Factory instance = null;
        private MainViewModel mainViewModel;

        /// <summary>
        /// Creates Main Factory.
        /// </summary>
        public static Factory Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new Factory();
                }

                return instance;
            }
        }

        public MainViewModel GetMainViewModel()
        {
            if (mainViewModel == null)
            {
                mainViewModel = new MainViewModel();
            }

            return mainViewModel;
        }
    }
}
