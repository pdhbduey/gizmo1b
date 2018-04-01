// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Threading;
    using System.Threading.Tasks;

    using Common;
    using Common.Bindings;
    using DeviceManager.Model;

    public class ThermistorViewModel : BindableBase
    {
        private IThermistorModel thermistorModel;

        private float ainA;
        private float ainB;
        private float ainC;
        private float ainD;
        private string statusMessage;

        private Task updateTask;
        CancellationTokenSource cts;
        CancellationToken token;
        private int updateDelay = 300;

        public ThermistorViewModel(IThermistorModel thermistorModel)
        {
            this.thermistorModel = thermistorModel;

            StartUpdateTask();
        }

        public string AinAText
        {
            get
            {
                return $"AIN__A: {ainA} °C";
            }
        }

        public float AinA
        {
            get
            {
                return ainA;
            }

            set
            {
                ainA = value;
                OnPropertyChanged(nameof(AinA));
                OnPropertyChanged(nameof(AinAText));
            }
        }

        public string AinBText
        {
            get
            {
                return $"AIN__B: {ainB} °C";
            }
        }

        public float AinB
        {
            get
            {
                return ainB;
            }

            set
            {
                ainB = value;
                OnPropertyChanged(nameof(AinB));
                OnPropertyChanged(nameof(AinBText));
            }
        }

        public string AinCText
        {
            get
            {
                return $"AIN__C: {ainC} °C";
            }
        }

        public float AinC
        {
            get
            {
                return ainC;
            }

            set
            {
                ainC = value;
                OnPropertyChanged(nameof(AinC));
                OnPropertyChanged(nameof(AinCText));
            }
        }

        public string AinDText
        {
            get
            {
                return $"AIN__D: {ainD} °C";
            }
        }

        public float AinD
        {
            get
            {
                return ainD;
            }

            set
            {
                ainD = value;
                OnPropertyChanged(nameof(AinD));
                OnPropertyChanged(nameof(AinDText));
            }
        }

        public string StatusMessage
        {
            get
            {
                return $"Status: {statusMessage}";
            }

            set
            {
                statusMessage = value;
                OnPropertyChanged(nameof(StatusMessage));
            }
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

                try
                {
                    var ainAData = await thermistorModel.ReadAinA();
                    AinA = Helper.GetFloatFromBigEndian(ainAData);
                    Thread.Sleep(updateDelay);

                    var ainBData = await thermistorModel.ReadAinB();
                    AinB = Helper.GetFloatFromBigEndian(ainBData);
                    Thread.Sleep(updateDelay);

                    var ainCData = await thermistorModel.ReadAinC();
                    AinC = Helper.GetFloatFromBigEndian(ainCData);
                    Thread.Sleep(updateDelay);

                    var ainDData = await thermistorModel.ReadAinD();
                    AinD = Helper.GetFloatFromBigEndian(ainDData);
                    Thread.Sleep(updateDelay);

                    var status = await thermistorModel.ReadStatus();
                    ProcessStatus(status);
                    Thread.Sleep(updateDelay);
                }
                catch (Exception e)
                {
                    StatusMessage = e.Message;
                }
            }
        }

        private void ProcessStatus(byte[] status)
        {
            if (status.Length < 4)
            {
                statusMessage = "Response length incorrect";
                return;
            }

            StatusMessage = GetErrorMessage(status[4]);
        }

        private string GetErrorMessage(byte value)
        {
            string response;
            ThermistorDefaults.Errors.TryGetValue(value, out response);

            return response == null ? "Unknown" : response;
        }
    }
}
