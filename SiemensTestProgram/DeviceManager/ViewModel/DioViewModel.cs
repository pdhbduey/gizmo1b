// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common.Bindings;
    using DeviceManager.Model;
    using System;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Windows;

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

        private const int updateDelay = 300;

        public DioViewModel(IDioModel dioModel)
        {
            this.dioModel = dioModel;

            doutZeroStatus = "Set Dout 0";
            doutOneStatus = "Set Dout 1";
            doutTwoStatus = "Set Dout 2";
            doutThreeStatus = "Set Dout 3";
            doutFourStatus = "Set Dout 4";
            doutFiveStatus = "Set Dout 5";
            doutSixStatus = "Set Dout 6";
            doutSevenStatus = "Set Dout 7";

            dinZeroColour = DioDefaults.notSetColour;
            dinOneColour = DioDefaults.notSetColour;
            dinTwoColour = DioDefaults.notSetColour;
            dinThreeColour = DioDefaults.notSetColour;
            dinFourColour = DioDefaults.notSetColour;
            dinFiveColour = DioDefaults.notSetColour;
            dinSixColour = DioDefaults.notSetColour;
            dinSevenColour = DioDefaults.notSetColour;
            dinEightColour = DioDefaults.notSetColour;
            dinNineColour = DioDefaults.notSetColour;

            // Initial updates and bindings
            Update();

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
            RefreshCommand = new RelayCommand(param => Update());
        }

        private void StartUpdateTask()
        {
            var thread = new Thread(() =>
            {
                UpdateAllStatuses();
            });

            thread.SetApartmentState(ApartmentState.STA);
            thread.Start();
        }

        private async void UpdateAllStatuses()
        {
            while (true)
            {
                var din = await dioModel.ReadDin();
                if (din.succesfulResponse)
                {
                    var zeroSet = DioDefaults.IsDinSet(din.response, 0);
                    var oneSet = DioDefaults.IsDinSet(din.response, 1);
                    var twoSet = DioDefaults.IsDinSet(din.response, 2);
                    var threeSet = DioDefaults.IsDinSet(din.response, 3);
                    var fourSet = DioDefaults.IsDinSet(din.response, 4);
                    var fiveSet = DioDefaults.IsDinSet(din.response, 5);
                    var sixSet = DioDefaults.IsDinSet(din.response, 6);
                    var sevenSet = DioDefaults.IsDinSet(din.response, 7);

                    updateDinStatus(0, zeroSet);
                    updateDinStatus(1, oneSet);
                    updateDinStatus(2, twoSet);
                    updateDinStatus(3, threeSet);
                    updateDinStatus(4, fourSet);
                    updateDinStatus(5, fiveSet);
                    updateDinStatus(6, sixSet);
                    updateDinStatus(7, sevenSet);
                }

                Thread.Sleep(updateDelay);
            }
        }

        private async void SetDoutCommand(int channel, string status)
        {
            var response = new byte[5];
            if (status.Equals(DioDefaults.SetDout))
            {
                await dioModel.SetDout(channel, set: true);
            }
            else
            {
                await dioModel.SetDout(channel, set: false);
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

        public RelayCommand RefreshCommand { get; set; }

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

        private bool isDoutZeroSet;
        public bool IsDoutZeroSet
        {
            get
            {
                return isDoutZeroSet;
            }

            set
            {
                isDoutZeroSet = value;
                OnPropertyChanged(nameof(IsDoutZeroSet));
            }
        }

        private bool isDoutOneSet;
        public bool IsDoutOneSet
        {
            get
            {
                return isDoutOneSet;
            }

            set
            {
                isDoutOneSet = value;
                OnPropertyChanged(nameof(IsDoutOneSet));
            }
        }

        private bool isDoutTwoSet;
        public bool IsDoutTwoSet
        {
            get
            {
                return isDoutTwoSet;
            }

            set
            {
                isDoutTwoSet = value;
                OnPropertyChanged(nameof(IsDoutTwoSet));
            }
        }

        private bool isDoutThreeSet;
        public bool IsDoutThreeSet
        {
            get
            {
                return isDoutThreeSet;
            }

            set
            {
                isDoutThreeSet = value;
                OnPropertyChanged(nameof(IsDoutThreeSet));
            }
        }

        private bool isDoutFourSet;
        public bool IsDoutFourSet
        {
            get
            {
                return isDoutFourSet;
            }

            set
            {
                isDoutFourSet = value;
                OnPropertyChanged(nameof(IsDoutFourSet));
            }
        }

        private bool isDoutFiveSet;
        public bool IsDoutFiveSet
        {
            get
            {
                return isDoutFiveSet;
            }

            set
            {
                isDoutFiveSet = value;
                OnPropertyChanged(nameof(IsDoutFiveSet));
            }
        }

        private bool isDoutSixSet;
        public bool IsDoutSixSet
        {
            get
            {
                return isDoutSixSet;
            }

            set
            {
                isDoutSixSet = value;
                OnPropertyChanged(nameof(IsDoutSixSet));
            }
        }

        private bool isDoutSevenSet;
        public bool IsDoutSevenSet
        {
            get
            {
                return isDoutSevenSet;
            }

            set
            {
                isDoutSevenSet = value;
                OnPropertyChanged(nameof(IsDoutSevenSet));
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

        private void Update()
        {
            var din = dioModel.ReadDin().Result;
            if (din.succesfulResponse)
            {
                var zeroSet = DioDefaults.IsDinSet(din.response, 0);
                var oneSet = DioDefaults.IsDinSet(din.response, 1);
                var twoSet = DioDefaults.IsDinSet(din.response, 2);
                var threeSet = DioDefaults.IsDinSet(din.response, 3);
                var fourSet = DioDefaults.IsDinSet(din.response, 4);
                var fiveSet = DioDefaults.IsDinSet(din.response, 5);
                var sixSet = DioDefaults.IsDinSet(din.response, 6);
                var sevenSet = DioDefaults.IsDinSet(din.response, 7);

                updateDinStatus(0, zeroSet);
                updateDinStatus(1, oneSet);
                updateDinStatus(2, twoSet);
                updateDinStatus(3, threeSet);
                updateDinStatus(4, fourSet);
                updateDinStatus(5, fiveSet);
                updateDinStatus(6, sixSet);
                updateDinStatus(7, sevenSet);
            }

            var dout = dioModel.ReadDout().Result;
            if (dout.succesfulResponse)
            {
                DoutSet(dout.response);
            }
        }

        private void DoutSet(byte[] doutData)
        {
            var dioZeroStatus = !DioDefaults.IsDoutSet(doutData, 0);
            var dioOneStatus = !DioDefaults.IsDoutSet(doutData, 1);
            var dioTwoStatus = !DioDefaults.IsDoutSet(doutData, 2);
            var dioThreeStatus = !DioDefaults.IsDoutSet(doutData, 3);
            var dioFourStatus = !DioDefaults.IsDoutSet(doutData, 4);
            var dioFiveStatus = !DioDefaults.IsDoutSet(doutData, 5);
            var dioSixStatus = !DioDefaults.IsDoutSet(doutData, 6);
            var dioSevenStatus = !DioDefaults.IsDoutSet(doutData, 7);

            IsDoutZeroSet = dioZeroStatus;
            IsDoutOneSet = dioOneStatus;
            IsDoutTwoSet = dioTwoStatus;
            IsDoutThreeSet = dioThreeStatus;
            IsDoutFourSet = dioFourStatus;
            IsDoutFiveSet = dioFiveStatus;
            IsDoutSixSet = dioSixStatus;
            IsDoutSevenSet = dioSevenStatus;

            if (dioZeroStatus)
            {
                DoutZeroStatus = DioDefaults.SetDout;
            }
            else
            {
                DoutZeroStatus = DioDefaults.ClearDout;
            }

            if (dioOneStatus)
            {
                DoutOneStatus = DioDefaults.SetDout;
            }
            else
            {
                DoutOneStatus = DioDefaults.ClearDout;
            }

            if (dioTwoStatus)
            {
                DoutTwoStatus = DioDefaults.SetDout;
            }
            else
            {
                DoutTwoStatus = DioDefaults.ClearDout;
            }

            if (dioThreeStatus)
            {
                DoutThreeStatus = DioDefaults.SetDout;
            }
            else
            {
                DoutThreeStatus = DioDefaults.ClearDout;
            }

            if (dioFourStatus)
            {
                DoutFourStatus = DioDefaults.SetDout;
            }
            else
            {
                DoutFourStatus = DioDefaults.ClearDout;
            }

            if (dioFiveStatus)
            {
                DoutFiveStatus = DioDefaults.SetDout;
            }
            else
            {
                DoutFiveStatus = DioDefaults.ClearDout;
            }

            if (dioSixStatus)
            {
                DoutSixStatus = DioDefaults.SetDout;
            }
            else
            {
                DoutSixStatus = DioDefaults.ClearDout;
            }

            if (dioSevenStatus)
            {
                DoutSevenStatus = DioDefaults.SetDout;
            }
            else
            {
                DoutSevenStatus = DioDefaults.ClearDout;
            }
        }

        private async void updateDinStatus(int channel, bool isSet)
        {
            switch (channel)
            {
                case 0:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinZeroColour = DioDefaults.setColour;
                        }));
                        
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinZeroColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
                case 1:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinOneColour = DioDefaults.setColour;
                        }));
                        
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinOneColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
                case 2:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinTwoColour = DioDefaults.setColour;
                        }));
                        
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinTwoColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
                case 3:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinThreeColour = DioDefaults.setColour;
                        }));
                       
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinThreeColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
                case 4:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinFourColour = DioDefaults.setColour;
                        }));
                        
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinFourColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
                case 5:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinFiveColour = DioDefaults.setColour;
                        }));
                        
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinFiveColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
                case 6:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinSixColour = DioDefaults.setColour;
                        }));
                        
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinSixColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
                case 7:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinSevenColour = DioDefaults.setColour;
                        }));
                        
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinSevenColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
                case 8:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinEightColour = DioDefaults.setColour;
                        }));
                        
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinEightColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
                case 9:
                    if (isSet)
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            DinNineColour = DioDefaults.setColour;
                        }));
                        
                        break;
                    }

                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        DinNineColour = DioDefaults.notSetColour;
                    }));
                    
                    break;
            }
        }
    }
}
