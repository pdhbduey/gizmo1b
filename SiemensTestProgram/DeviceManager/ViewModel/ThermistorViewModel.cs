// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Collections.Generic;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Windows;
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
        private string selectedType;
        private const int updateDelay = 300;

        public ThermistorViewModel(IThermistorModel thermistorModel)
        {
            this.thermistorModel = thermistorModel;

            Types = ThermistorDefaults.Types;
            SelectedType = Types[1];
            InitialUpdate();
            StartUpdateTask();
        }

        public List<string> Types { get; set; }

        public string SelectedType
        {
            get
            {
                return selectedType;
            }
            set
            {
                selectedType = value;
                OnPropertyChanged(nameof(SelectedType));

                SetType();
            }
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
                var ainAData = await thermistorModel.ReadAinA();
                if (ainAData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        AinA = Helper.GetFloatFromBigEndian(ainAData.response);
                    }));
                    
                }

                Thread.Sleep(updateDelay);

                var ainBData = await thermistorModel.ReadAinB();
                if (ainBData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        AinB = Helper.GetFloatFromBigEndian(ainBData.response);
                    }));
                    
                }

                Thread.Sleep(updateDelay);

                var ainCData = await thermistorModel.ReadAinC();
                if (ainCData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        AinC = Helper.GetFloatFromBigEndian(ainCData.response);
                    }));
                    
                }

                Thread.Sleep(updateDelay);

                var ainDData = await thermistorModel.ReadAinD();
                if (ainDData.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        AinD = Helper.GetFloatFromBigEndian(ainDData.response);
                    }));
                }

                Thread.Sleep(updateDelay);

                var status = await thermistorModel.ReadStatus();
                if (status.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        ProcessStatus(status.response);
                    }));
                }

                Thread.Sleep(updateDelay);
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
            if (ainAData.succesfulResponse)
            {
                AinA = Helper.GetFloatFromBigEndian(ainAData.response);
            }

            var ainBData = thermistorModel.ReadAinB().Result;
            if (ainBData.succesfulResponse)
            {
                AinB = Helper.GetFloatFromBigEndian(ainBData.response);
            }

            var ainCData = thermistorModel.ReadAinC().Result;
            if (ainCData.succesfulResponse)
            {
                AinC = Helper.GetFloatFromBigEndian(ainCData.response);
            }

            var ainDData = thermistorModel.ReadAinD().Result;
            if (ainDData.succesfulResponse)
            {
                AinD = Helper.GetFloatFromBigEndian(ainDData.response);
            }

            var status = thermistorModel.ReadStatus().Result;
            if (status.succesfulResponse)
            {
                ProcessStatus(status.response);
            }
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

        private void SetType()
        {
            thermistorModel.SetType(selectedType);
        }
    }
}
