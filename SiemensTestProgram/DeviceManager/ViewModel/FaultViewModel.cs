// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common;
    using Common.Bindings;
    using Model;
    using System.Drawing;
    using System.Threading;
    using System.Threading.Tasks;

    public class FaultViewModel : BindableBase
    {
        private string notSetColour = "Gray";
        private string setColour = "Green";
        private string ntcOneColour;
        private string ntcTwoColour;
        private string tecOcdPosColour;
        private string tecOcdNegColour;
        private string overtempOneColour;
        private string overtempTwoColour;
        private const int updateDelay = 300;
        private IFaultModel faultModel;

        public FaultViewModel(IFaultModel faultModel)
        {
            this.faultModel = faultModel;

            tecOcdNegColour = notSetColour;
            tecOcdPosColour = notSetColour;
            overtempOneColour = notSetColour;
            overtempTwoColour = notSetColour;
            ntcOneColour = notSetColour;
            ntcTwoColour = notSetColour;

            ResetCommand = new RelayCommand(param => Reset());
            RefreshCommand = new RelayCommand(param => Update());

            GetNtc();
            GetState();
            StartUpdateTask();
        }

        public RelayCommand RefreshCommand { get; set; }

        private void Update()
        {
            GetNtc();
            GetState();
        }

        private void StartUpdateTask()
        {
            Task.Factory.StartNew(() =>
            {
                UpdateAllStatuses();
            });
        }

        private async void UpdateAllStatuses()
        {
            while (true)
            {
                //var state = await faultModel.GetState();
                //if (state.succesfulResponse)
                //{
                //    overtempOneColour = notSetColour;
                //    overtempTwoColour = notSetColour;
                //    tecOcdPosColour = notSetColour;
                //    tecOcdNegColour = notSetColour;


                //    if (Helper.IsBitSet(state.response[3], 0))
                //    {
                //        tecOcdNegColour = setColour;
                //    }

                //    if (Helper.IsBitSet(state.response[3], 1))
                //    {
                //        tecOcdPosColour = setColour;
                //    }

                //    if (Helper.IsBitSet(state.response[3], 2))
                //    {
                //        overtempOneColour = setColour;
                //    }

                //    if (Helper.IsBitSet(state.response[3], 3))
                //    {
                //        overtempTwoColour = setColour;
                //    }

                //    OnPropertyChanged(nameof(OvertempOneColour));
                //    OnPropertyChanged(nameof(OvertempTwoColour));
                //    OnPropertyChanged(nameof(TecOcdPosColour));
                //    OnPropertyChanged(nameof(TecOcdNegColour));
                //}
                //Thread.Sleep(updateDelay);

                var ntcState = await faultModel.GetNtcStatus();
                if (ntcState.succesfulResponse)
                {
                    if (Helper.IsBitSet(ntcState.response[3], 0))
                    {
                        NtcOneColour = setColour;
                    }
                    else
                    {
                        NtcOneColour = notSetColour;
                    }

                    if (Helper.IsBitSet(ntcState.response[3], 1))
                    {
                        NtcTwoColour = setColour;
                    }
                    else
                    {
                        NtcTwoColour = notSetColour;
                    }
                }

                Thread.Sleep(updateDelay);
            }
        }

        public RelayCommand ResetCommand { get; set; }

        public string NtcOneColour
        {
            get
            {
                return ntcOneColour;
            }

            set
            {
                ntcOneColour = value;
                OnPropertyChanged(nameof(NtcOneColour));
            }
        }

        public string NtcTwoColour
        {
            get
            {
                return ntcTwoColour;
            }

            set
            {
                ntcTwoColour = value;
                OnPropertyChanged(nameof(NtcTwoColour));
            }
        }

        public string TecOcdNegColour
        {
            get
            {
                return tecOcdNegColour;
            }

            set
            {
                tecOcdNegColour = value;
                OnPropertyChanged(nameof(TecOcdNegColour));
            }
        }

        public string TecOcdPosColour
        {
            get
            {
                return tecOcdPosColour;
            }

            set
            {
                tecOcdPosColour = value;
                OnPropertyChanged(nameof(TecOcdPosColour));
            }
        }

        public string OvertempOneColour
        {
            get
            {
                return overtempOneColour;
            }

            set
            {
                overtempOneColour = value;
                OnPropertyChanged(nameof(OvertempOneColour));
            }
        }

        public string OvertempTwoColour
        {
            get
            {
                return overtempTwoColour;
            }

            set
            {
                overtempTwoColour = value;
                OnPropertyChanged(nameof(OvertempTwoColour));
            }
        }

        private void Reset()
        {
            var response = new byte[5];
            faultModel.Reset(ref response);
        }

        private void GetNtc()
        {
            var ntcState = new byte[5];
            if (faultModel.GetNtcStatus(ref ntcState))
            {
                ntcOneColour = notSetColour;
                ntcTwoColour = notSetColour;

                if (Helper.IsBitSet(ntcState[3], 0))
                {
                    ntcOneColour = setColour;
                }

                if (Helper.IsBitSet(ntcState[3], 1))
                {
                    ntcTwoColour = setColour;
                }

                OnPropertyChanged(nameof(NtcOneColour));
                OnPropertyChanged(nameof(NtcTwoColour));
            }
        }

        private void GetState()
        {
            var state = new byte[5];
            if (faultModel.GetState(ref state))
            {
                overtempOneColour = notSetColour;
                overtempTwoColour = notSetColour;
                tecOcdPosColour = notSetColour;
                tecOcdNegColour = notSetColour;


                if (Helper.IsBitSet(state[3], 0))
                {
                    tecOcdNegColour = setColour;
                }

                if (Helper.IsBitSet(state[3], 1))
                {
                    tecOcdPosColour = setColour;
                }

                if (Helper.IsBitSet(state[3], 2))
                {
                    overtempOneColour = setColour;
                }

                if (Helper.IsBitSet(state[3], 3))
                {
                    overtempTwoColour = setColour;
                }

                OnPropertyChanged(nameof(OvertempOneColour));
                OnPropertyChanged(nameof(OvertempTwoColour));
                OnPropertyChanged(nameof(TecOcdPosColour));
                OnPropertyChanged(nameof(TecOcdNegColour));
            }
        }
    }
}
