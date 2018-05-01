// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Threading;
    using System.Threading.Tasks;

    using Common;
    using Common.Bindings;
    using DeviceManager.Model;

    /// <summary>
    /// This class is responsible for updating Thermistor.
    /// </summary>
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
        private int updateDelay = 500;
        private int delayBetweenRequests = 50;

        public ThermistorViewModel(IThermistorModel thermistorModel)
        {
            this.thermistorModel = thermistorModel;
            InitialUpdate();

            // Update Thermistor status values.
            StartUpdateTask();
        }

        /// <summary>
        /// Text value of AIN_A.
        /// </summary>
        public string AinAText
        {
            get
            {
                return $"AIN__A: {ainA.ToString("0.##")} °C";
            }
        }

        /// <summary>
        /// Value for AIN_A.
        /// </summary>
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

        /// <summary>
        /// Text value of AIN_B.
        /// </summary>
        public string AinBText
        {
            get
            {
                return $"AIN__B: {ainB.ToString("0.##")} °C";
            }
        }

        /// <summary>
        /// Value for AIN_B.
        /// </summary>
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

        /// <summary>
        /// Text value of AIN_C.
        /// </summary>
        public string AinCText
        {
            get
            {
                return $"AIN__C: {ainC.ToString("0.##")} °C";
            }
        }

        /// <summary>
        /// Value for AIN_C.
        /// </summary>
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

        /// <summary>
        /// Text value of AIN_D.
        /// </summary>
        public string AinDText
        {
            get
            {
                return $"AIN__D: {ainD.ToString("0.##")} °C";
            }
        }

        /// <summary>
        /// Value for AIN_D.
        /// </summary>
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

        /// <summary>
        /// Returns the status of the thermistor.
        /// </summary>
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

        /// <summary>
        /// Task that reads and updates text for responses for thermistor outputs.
        /// </summary>
        private void StartUpdateTask()
        {
            cts = new CancellationTokenSource();
            token = cts.Token;

            updateTask = Task.Factory.StartNew(() =>
            {
                UpdateAllStatuses();
            }, token);
        }

        /// <summary>
        /// Updates Thermistor values.
        /// </summary>
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
                    Thread.Sleep(delayBetweenRequests);

                    var ainBData = await thermistorModel.ReadAinB();
                    AinB = Helper.GetFloatFromBigEndian(ainBData);
                    Thread.Sleep(delayBetweenRequests);

                    var ainCData = await thermistorModel.ReadAinC();
                    AinC = Helper.GetFloatFromBigEndian(ainCData);
                    Thread.Sleep(delayBetweenRequests);

                    var ainDData = await thermistorModel.ReadAinD();
                    AinD = Helper.GetFloatFromBigEndian(ainDData);
                    Thread.Sleep(delayBetweenRequests);

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

        /// <summary>
        /// Processes read response for thermistor.
        /// </summary>
        /// <param name="status"> Register response. </param>
        private void ProcessStatus(byte[] status)
        {
            if (status.Length < 4)
            {
                StatusMessage = "Communication Error";
                return;
            }

            StatusMessage = GetErrorMessage(status[4]);
        }

        private void InitialUpdate()
        {
            var ainAData = thermistorModel.ReadAinA().Result;
            AinA = Helper.GetFloatFromBigEndian(ainAData);
            Thread.Sleep(50);

            var ainBData = thermistorModel.ReadAinB().Result;
            AinB = Helper.GetFloatFromBigEndian(ainBData);
            Thread.Sleep(50);

            var ainCData = thermistorModel.ReadAinC().Result;
            AinC = Helper.GetFloatFromBigEndian(ainCData);
            Thread.Sleep(50);

            var ainDData = thermistorModel.ReadAinD().Result;
            AinD = Helper.GetFloatFromBigEndian(ainDData);
            Thread.Sleep(50);

            var status = thermistorModel.ReadStatus().Result;
            ProcessStatus(status);
            Thread.Sleep(50);
        }

        /// <summary>
        /// Checks text for register response.
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        private string GetErrorMessage(byte value)
        {
            string response;
            ThermistorDefaults.Errors.TryGetValue(value, out response);

            return response == null ? "Unknown" : response;
        }
    }
}
