
namespace DeviceManager.ViewModel
{
    using Common.Bindings;
    using DeviceManager.Model;
    using System.Threading;
    using System.Threading.Tasks;

    public class DioViewModel : BindableBase
    {
        private IDioModel dioModel;

        private string dinZeroColour;
        private string dinOneColour;
        private string dinTwoColour;
        private string dinThreeColour;
        private string dinFourColour;
        private string dinFiveColour;
        private string dinSixColour;
        private string dinSevenColour;
        private string dinEightColour;
        private string dinNineColour;

        private string doutZeroStatus;
        private string doutOneStatus;
        private string doutTwoStatus;
        private string doutThreeStatus;
        private string doutFourStatus;
        private string doutFiveStatus;
        private string doutSixStatus;
        private string doutSevenStatus;

        private int updateDelay = 1500;
        private Task updateTask;

        public DioViewModel(IDioModel dioModel)
        {
            this.dioModel = dioModel;

            // Initial updates and bindings
            InitialUpdate();
            
            SetDioZeroCommand = new RelayCommand(param => SetDoutCommand(channel: 0, status: doutZeroStatus));
            SetDioOneCommand = new RelayCommand(param => SetDoutCommand(channel: 1, status: doutOneStatus));
            SetDioTwoCommand = new RelayCommand(param => SetDoutCommand(channel: 2, status: doutTwoStatus));
            SetDioThreeCommand = new RelayCommand(param => SetDoutCommand(channel: 3, status: doutThreeStatus));
            SetDioFourCommand = new RelayCommand(param => SetDoutCommand(channel: 4, status: doutFourStatus));
            SetDioFiveCommand = new RelayCommand(param => SetDoutCommand(channel: 5, status: doutFiveStatus));
            SetDioSixCommand = new RelayCommand(param => SetDoutCommand(channel: 6, status: doutSixStatus));
            SetDioSevenCommand = new RelayCommand(param => SetDoutCommand(channel: 7, status: doutSevenStatus));
            
            // Update Din values.
            StartUpdateTask();
        }

        private async void SetDoutCommand(int channel, string status)
        {
            if (status.Contains("Set"))
            {
                //await dioModel.SetDout(channel, set: true);
            }
            else
            {
                //await dioModel.SetDout(channel, set: false);
            }

            UpdateChannelDout(channel);
        }

        private void UpdateChannelDout(int channel)
        {
            switch (channel)
            {
                case 0:
                    DoutZeroStatus = DoutStatusChange(doutZeroStatus);
                    break;
                case 1:
                    DoutOneStatus = DoutStatusChange(doutOneStatus);
                    break;
                case 2:
                    DoutTwoStatus = DoutStatusChange(doutTwoStatus);
                    break;
                case 3:
                    DoutThreeStatus = DoutStatusChange(doutThreeStatus);
                    break;
                case 4:
                    DoutFourStatus = DoutStatusChange(doutFourStatus);
                    break;
                case 5:
                    DoutFiveStatus = DoutStatusChange(doutFiveStatus);
                    break;
                case 6:
                    DoutSixStatus = DoutStatusChange(doutSixStatus);
                    break;
                case 7:
                    DoutSevenStatus = DoutStatusChange(doutSevenStatus);
                    break;
            }
        }

        private string DoutStatusChange(string status)
        {
            var statusValue = status.Contains("Set") ? status.Replace("Set", "Clear") : status.Replace("Clear", "Set");
            return statusValue;
        }

        public RelayCommand SetDioZeroCommand { get; set; }

        public RelayCommand SetDioOneCommand { get; set; }

        public RelayCommand SetDioTwoCommand { get; set; }

        public RelayCommand SetDioThreeCommand { get; set; }

        public RelayCommand SetDioFourCommand { get; set; }

        public RelayCommand SetDioFiveCommand { get; set; }

        public RelayCommand SetDioSixCommand { get; set; }

        public RelayCommand SetDioSevenCommand { get; set; }

        public string DoutZeroStatus
        {
            get
            {
                return doutZeroStatus;
            }

            set
            {
                doutZeroStatus = value;
                OnPropertyChanged(nameof(DoutZeroStatus));
            }
        }

        public string DoutOneStatus
        {
            get
            {
                return doutOneStatus;
            }

            set
            {
                doutOneStatus = value;
                OnPropertyChanged(nameof(DoutOneStatus));
            }
        }

        public string DoutTwoStatus
        {
            get
            {
                return doutTwoStatus;
            }

            set
            {
                doutTwoStatus = value;
                OnPropertyChanged(nameof(DoutTwoStatus));
            }
        }

        public string DoutThreeStatus
        {
            get
            {
                return doutThreeStatus;
            }

            set
            {
                doutThreeStatus = value;
                OnPropertyChanged(nameof(DoutThreeStatus));
            }
        }

        public string DoutFourStatus
        {
            get
            {
                return doutFourStatus;
            }

            set
            {
                doutFourStatus = value;
                OnPropertyChanged(nameof(DoutFourStatus));
            }
        }

        public string DoutFiveStatus
        {
            get
            {
                return doutFiveStatus;
            }

            set
            {
                doutFiveStatus = value;
                OnPropertyChanged(nameof(DoutFiveStatus));
            }
        }

        public string DoutSixStatus
        {
            get
            {
                return doutSixStatus;
            }

            set
            {
                doutSixStatus = value;
                OnPropertyChanged(nameof(DoutSixStatus));
            }
        }

        public string DoutSevenStatus
        {
            get
            {
                return doutSevenStatus;
            }

            set
            {
                doutSevenStatus = value;
                OnPropertyChanged(nameof(DoutSevenStatus));
            }
        }

        public string DinZeroColour
        {
            get
            {
                return dinZeroColour;
            }
            set
            {
                dinZeroColour = value;
                OnPropertyChanged(nameof(DinZeroColour));
            }
        }

        public string DinOneColour
        {
            get
            {
                return dinOneColour;
            }
            set
            {
                dinOneColour = value;
                OnPropertyChanged(nameof(DinOneColour));
            }
        }

        public string DinTwoColour
        {
            get
            {
                return dinTwoColour;
            }
            set
            {
                dinTwoColour = value;
                OnPropertyChanged(nameof(DinTwoColour));
            }
        }

        public string DinThreeColour
        {
            get
            {
                return dinThreeColour;
            }
            set
            {
                dinThreeColour = value;
                OnPropertyChanged(nameof(DinThreeColour));
            }
        }

        public string DinFourColour
        {
            get
            {
                return dinFourColour;
            }
            set
            {
                dinFourColour = value;
                OnPropertyChanged(nameof(DinFourColour));
            }
        }

        public string DinFiveColour
        {
            get
            {
                return dinFiveColour;
            }
            set
            {
                dinFiveColour = value;
                OnPropertyChanged(nameof(DinFiveColour));
            }
        }

        public string DinSixColour
        {
            get
            {
                return dinSixColour;
            }
            set
            {
                dinSixColour = value;
                OnPropertyChanged(nameof(DinSixColour));
            }
        }

        public string DinSevenColour
        {
            get
            {
                return dinSevenColour;
            }
            set
            {
                dinSevenColour = value;
                OnPropertyChanged(nameof(DinSevenColour));
            }
        }

        public string DinEightColour
        {
            get
            {
                return dinEightColour;
            }
            set
            {
                dinEightColour = value;
                OnPropertyChanged(nameof(DinEightColour));
            }
        }

        public string DinNineColour
        {
            get
            {
                return dinNineColour;
            }
            set
            {
                dinNineColour = value;
                OnPropertyChanged(nameof(DinNineColour));
            }
        }

        private void InitialUpdate()
        {
            dinZeroColour = "Green";
            dinOneColour = "Green";
            dinTwoColour = "Green";
            dinThreeColour = "Green";
            dinFourColour = "Green";
            dinFiveColour = "Green";
            dinSixColour = "Green";
            dinSevenColour = "Green";
            dinEightColour = "Green";
            dinNineColour = "Red";

            doutZeroStatus = "Set Dout 0";
            doutOneStatus = "Set Dout 1";
            doutTwoStatus = "Set Dout 2";
            doutThreeStatus = "Set Dout 3";
            doutFourStatus = "Set Dout 4";
            doutFiveStatus = "Set Dout 5";
            doutSixStatus = "Set Dout 6";
            doutSevenStatus = "Set Dout 7";
        }

        private void DinUpdate()
        {
            while (true)
            {
                try
                {
                    

                    Thread.Sleep(updateDelay);
                }
                catch
                {
                    
                }
            }
        }

        /// <summary>
        /// Task that reads and updates colours for Din values.
        /// </summary>
        private void StartUpdateTask()
        {
            updateTask = Task.Factory.StartNew(() =>
            {
                DinUpdate();
            });
        }
    }
}
