// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Windows;

    using Common;
    using Common.Bindings;
    using Model;

    public class FaultViewModel : BindableBase, IDisposable
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

        private Task updateTask;
        private CancellationTokenSource cts;
        private CancellationToken token;
        private const int updateDelay = 300;

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

            Update();
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
            cts = new CancellationTokenSource();
            token = cts.Token;

            updateTask = Task.Factory.StartNew(() =>
            {
                UpdateAllStatuses();
            }, token);
        }

        private async void UpdateAllStatuses()
        {
            while (true)
            {
                if (token.IsCancellationRequested == true)
                {
                    break;
                }

                var state = await faultModel.GetState();
                if (state.succesfulResponse)
                {
                    if (Helper.IsBitSet(state.response[3], 0))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TecOcdNegColour = setColour;
                        }));
                        
                    }
                    else
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TecOcdNegColour = notSetColour;
                        }));
                        
                    }

                    if (Helper.IsBitSet(state.response[3], 1))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TecOcdPosColour = setColour;
                        }));
                        
                    }
                    else
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TecOcdPosColour = notSetColour;
                        }));
                        
                    }

                    if (Helper.IsBitSet(state.response[3], 2))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            OvertempOneColour = setColour;
                        }));
                        
                    }
                    else
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            OvertempOneColour = notSetColour;
                        }));
                        
                    }

                    if (Helper.IsBitSet(state.response[3], 3))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            OvertempTwoColour = setColour;
                        }));
                        
                    }
                    else
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            OvertempTwoColour = notSetColour;
                        }));
                        
                    }
                }
                Thread.Sleep(updateDelay);

                var ntcState = await faultModel.GetNtcStatus();
                if (ntcState.succesfulResponse)
                {
                    if (Helper.IsBitSet(ntcState.response[3], 0))
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            NtcOneColour = setColour;
                        }));
                    }
                    else
                    {
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            NtcOneColour = notSetColour;
                        }));
                    }

                    if (Helper.IsBitSet(ntcState.response[3], 1))
                    {
                        
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            NtcTwoColour = setColour;
                        }));
                    }
                    else
                    {
                        
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            NtcTwoColour = notSetColour;
                        }));
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

        private async void Reset()
        {
            await faultModel.Reset();
        }

        private async void GetNtc()
        {
            var ntcState = await faultModel.GetNtcStatus();
            if (ntcState.succesfulResponse)
            {
                ntcOneColour = notSetColour;
                ntcTwoColour = notSetColour;

                if (Helper.IsBitSet(ntcState.response[3], 0))
                {
                    ntcOneColour = setColour;
                }

                if (Helper.IsBitSet(ntcState.response[3], 1))
                {
                    ntcTwoColour = setColour;
                }

                OnPropertyChanged(nameof(NtcOneColour));
                OnPropertyChanged(nameof(NtcTwoColour));
            }
        }

        private async void GetState()
        {
            var state = await faultModel.GetState();
            if (state.succesfulResponse)
            {
                overtempOneColour = notSetColour;
                overtempTwoColour = notSetColour;
                tecOcdPosColour = notSetColour;
                tecOcdNegColour = notSetColour;


                if (Helper.IsBitSet(state.response[3], 0))
                {
                    tecOcdNegColour = setColour;
                }

                if (Helper.IsBitSet(state.response[3], 1))
                {
                    tecOcdPosColour = setColour;
                }

                if (Helper.IsBitSet(state.response[3], 2))
                {
                    overtempOneColour = setColour;
                }

                if (Helper.IsBitSet(state.response[3], 3))
                {
                    overtempTwoColour = setColour;
                }

                OnPropertyChanged(nameof(OvertempOneColour));
                OnPropertyChanged(nameof(OvertempTwoColour));
                OnPropertyChanged(nameof(TecOcdPosColour));
                OnPropertyChanged(nameof(TecOcdNegColour));
            }
        }

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects).
                    cts?.Cancel();
                }

                // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
                updateTask = null;

                disposedValue = true;
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion
    }
}
