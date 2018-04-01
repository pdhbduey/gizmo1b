using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.ViewModel
{
    using Common.Bindings;
    using DeviceManager.Model;

    public class DeviceManagerViewModel : BindableBase
    {
        private string communicationType;
        private IDeviceManager deviceManager;

        public DeviceManagerViewModel(IDeviceManager deviceManager)
        {
            this.deviceManager = deviceManager;
            communicationType = string.Empty;
        }


        public string CommunicationType
        {
            get
            {
                return communicationType;
            }

            set
            {
                communicationType = value;
                OnPropertyChanged(nameof(CommunicationType));
            }
        }
    }
}
