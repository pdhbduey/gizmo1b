// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Collections.Generic;

    using Common.Bindings;
    using DeviceManager.DeviceCommunication;
    using DeviceManager.Model;

    public class CommunicationConfigurationViewModel : BindableBase, IDisposable
    { 
        public string selectedComPort;
        public int selectedBaudRate;
        public int dataBits;
        public System.IO.Ports.Parity selectedParity;
        public System.IO.Ports.StopBits selectedStopBits;
        private ICommunicationConfigurationModel communicationConfigurationModel;
        private string description;
        private string status;
        private string name;
        private string pnpDeviceId;
        private string configurationStatus;

        public CommunicationConfigurationViewModel(ICommunicationConfigurationModel communicationConfigurationModel)
        {
            this.communicationConfigurationModel = communicationConfigurationModel;
            ComPorts = new List<string>();
 
            // Sets default values
            ComPorts = communicationConfigurationModel.GetPortSettings();
            Parities = ComPortDefaults.Parities;
            BaudRates = ComPortDefaults.BaudRates;
            StopBits = ComPortDefaults.StopBits;
            configurationStatus = "Not Configured";
            communicationConfigurationModel.GetDefaultSettings(ref selectedComPort, ref selectedBaudRate, ref dataBits, ref selectedParity, ref selectedStopBits);
            GetDetailsForPort();

            ConfigureCommunicationCommand = new RelayCommand(param => ConfigureComCommunication());
        }

        public string ConfigurationStatus
        {
            get
            {
                return configurationStatus;
            }
            set
            {
                configurationStatus = value;
                OnPropertyChanged(nameof(ConfigurationStatus));
            }
        }

        public string SelectedComPort
        {
            get
            {
                return selectedComPort;
            }
            set
            {
                selectedComPort = value;
                GetDetailsForPort();
                OnPropertyChanged(nameof(SelectedComPort));
            }
        }

        public int SelectedBaudRate
        {
            get
            {
                return selectedBaudRate;
            }
            set
            {
                selectedBaudRate = value;
                OnPropertyChanged(nameof(SelectedBaudRate));
            }
        }
        public int DataBits
        {
            get
            {
                return dataBits;
            }
            set
            {
                dataBits = value;
                OnPropertyChanged(nameof(DataBits));
            }
        }

        public System.IO.Ports.Parity SelectedParity
        {
            get
            {
                return selectedParity;
            }
            set
            {
                selectedParity = value;
                OnPropertyChanged(nameof(SelectedParity));
            }
        }

        public System.IO.Ports.StopBits SelectedStopBits
        {
            get
            {
                return selectedStopBits;
            }
            set
            {
                selectedStopBits = value;
                OnPropertyChanged(nameof(SelectedStopBits));
            }
        }

        public RelayCommand ConfigureCommunicationCommand { get; set; }

        public List<string> ComPorts { get; set; }

        public List<int> BaudRates { get; set; }

        public List<System.IO.Ports.Parity> Parities { get; set; }

        public List<System.IO.Ports.StopBits> StopBits { get; set; }

        private void ConfigureComCommunication()
        {
            ConfigurationStatus = communicationConfigurationModel.ReconfigureComCommunication(selectedComPort, selectedBaudRate, dataBits, selectedParity, selectedStopBits);
        }

        public string Description
        {
            get
            {
                return "Description: " + description;
            }

            set
            {
                description = value;
                OnPropertyChanged(nameof(Description));
            }
        }

        public string PnpDeviceId
        {
            get
            {
                return "PNPDeviceId: " + pnpDeviceId;
            }

            set
            {
                pnpDeviceId = value;
                OnPropertyChanged(nameof(PnpDeviceId));
            }
        }

        public string Status
        {
            get
            {
                return "Status: " + status;
            }

            set
            {
                status = value;
                OnPropertyChanged(nameof(Status));
            }
        }

        public string Name
        {
            get
            {
                return "Name: " + name;
            }

            set
            {
                name = value;
                OnPropertyChanged(nameof(Name));
            }
        }

        private void GetDetailsForPort()
        {
            communicationConfigurationModel.GetDeviceInformationAtPort(selectedComPort, ref description, ref name, ref status, ref pnpDeviceId);
            OnPropertyChanged(nameof(Name));
            OnPropertyChanged(nameof(Status));
            OnPropertyChanged(nameof(PnpDeviceId));
            OnPropertyChanged(nameof(Description));
        }

        public void Dispose()
        {
            // do nothing
        }
    }
}
