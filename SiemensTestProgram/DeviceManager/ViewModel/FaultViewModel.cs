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

            GetNtc();
            GetState();
            StartUpdateTask();
        }

        private void StartUpdateTask()
        {
            Task.Factory.StartNew(() =>
            {
                UpdateAllStatuses();
            });
        }

        private void UpdateAllStatuses()
        {
            while (true)
            {
                GetState();
                Thread.Sleep(100);
                GetNtc();
                Thread.Sleep(100);
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

        private async void Reset()
        {
            await faultModel.Reset();
        }

        private void GetNtc()
        {
            var ntcState = faultModel.GetNtcStatus();
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

        private void GetState()
        {
            var state = faultModel.GetState();
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
