// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common;
    using Common.Bindings;
    using DeviceManager.Model;
    using System;
    using System.Collections.Generic;
    using System.Threading;
    using System.Threading.Tasks;

    public class MotorViewModel : BindableBase
    {
        private IMotorModel motorModel;
        private string selectedStepSize;
        private string selectedDirection;
        private string selectedRegisterAddress;
        private string registerWriteValue;
        private string motorPosition;
        private string registerReadValue;
        private string bridgeInHiz;
        private string busyStatus;
        private string errorStatus;
        private int absoluteMoveValue;
        private int relativeMoveValue;
        private byte[] registerValue;

        private const int updateDelay = 400;
        private CancellationTokenSource cts;
        private CancellationToken token;
        private Task updateTask;

        public MotorViewModel(IMotorModel motorModel)
        {
            this.motorModel = motorModel;

            //// initial values
            StepSizes = MotorDefaults.StepSizes;
            SelectedStepSize = StepSizes[0];

            Directions = MotorDefaults.Directions;
            SelectedDirection = Directions[0];

            RegisterAddresses = MotorDefaults.RegisterAddresses;
            SelectedRegisterAddress = RegisterAddresses[0];

            registerWriteValue = "00000000";
            AbsoluteMoveValue = 0;
            RelativeMoveValue = 0;

            //// Initial Update
            InitialUpdate();

            //SetConfigurationCommand = new RelayCommand(param => SetConfiguration());
            HomeCommand = new RelayCommand(param => Home());
            ResetCommand = new RelayCommand(param => Reset());
            LimpCommand = new RelayCommand(param => Limp());
            MoveAbsoluteCommand = new RelayCommand(param => MoveToAbsolutePosition());
            MoveRelativeCommand = new RelayCommand(param => MoveToRelativePosition());
            InitializeCommand = new RelayCommand(param => Initialize());
            CycleRelativeCommand = new RelayCommand(param => Cycle());
            StopCommand = new RelayCommand(param => Stop());
            SetRegisterValueCommand = new RelayCommand(param => SetRegister());
            ReadRegisterValueCommand = new RelayCommand(param => ReadRegister());
            RefreshCommand = new RelayCommand(param => RefreshMotorStatus());
            RefreshPositionCommand = new RelayCommand(param => RefreshPosition());

            //StartUpdateTask();
        }

        public RelayCommand RefreshCommand { get; set; }

        public RelayCommand RefreshPositionCommand { get; set; }

        public RelayCommand ReadRegisterValueCommand { get; set; }

        public RelayCommand SetRegisterValueCommand { get; set; }

        public RelayCommand CycleRelativeCommand { get; set; }

        public RelayCommand StopCommand { get; set; }

        public RelayCommand MoveRelativeCommand { get; set; }

        public RelayCommand MoveAbsoluteCommand { get; set; }

        public RelayCommand SetConfigurationCommand { get; set; }

        public RelayCommand ResetCommand { get; set; }

        public RelayCommand LimpCommand { get; set; }

        public RelayCommand HomeCommand { get; set; }

        public RelayCommand InitializeCommand { get; set; }

        public List<string> StepSizes { get; set; }

        public List<string> Directions { get; set; }

        public List<string> RegisterAddresses { get; set; }

        public string RegisterWriteValue
        {
            get
            {
                return registerWriteValue;
            }

            set
            {
                registerWriteValue = value;
                registerValue = Helper.ConvertStringToByteArray(registerWriteValue);
                OnPropertyChanged(nameof(RegisterWriteValue));
            }
        }

        public string SelectedRegisterAddress
        {
            get
            {
                return selectedRegisterAddress;
            }
            set
            {
                selectedRegisterAddress = value;
                OnPropertyChanged(nameof(SelectedRegisterAddress));

                SetSelectedRegisterAddress();
            }
        }

        public string SelectedStepSize
        {
            get
            {
                return selectedStepSize;
            }
            set
            {
                selectedStepSize = value;
                OnPropertyChanged(nameof(SelectedStepSize));
            }
        }

        public string SelectedDirection
        {
            get
            {
                return selectedDirection;
            }
            set
            {
                selectedDirection = value;
                OnPropertyChanged(nameof(SelectedDirection));
            }
        }

        public string MotorPosition
        {
            get
            {
                return motorPosition;
            }

            set
            {
                motorPosition = $"Position: {value}";
                OnPropertyChanged(nameof(MotorPosition));
            }
        }

        public string RegisterReadValue
        {
            get
            {
                return registerReadValue;
            }

            set
            {
                registerReadValue = $"Register Value: {value}";
                OnPropertyChanged(nameof(RegisterReadValue));
            }
        }

        public int AbsoluteMoveValue {
            get
            {
                return absoluteMoveValue;
            }
            set
            {
                absoluteMoveValue = value;
                OnPropertyChanged(nameof(AbsoluteMoveValue));
                SetAbsoluteMovePosition();
            }
        }

        public int RelativeMoveValue {
            get
            {
                return relativeMoveValue;
            }
            set
            {
                relativeMoveValue = value;
                OnPropertyChanged(nameof(RelativeMoveValue));
                SetRelativeMovePosition();
            }
        }

        public string BridgeInHiz
        {
            get
            {
                return bridgeInHiz;
            }
            set
            {
                bridgeInHiz = $"Bridge in Hiz: {value}";
                OnPropertyChanged(nameof(BridgeInHiz));
            }
        }

        public string BusyStatus
        {
            get
            {
                return busyStatus;
            }
            set
            {
                busyStatus = $"Busy: {value}";
                OnPropertyChanged(nameof(BusyStatus));
            }
        }

        public string ErrorStatus
        {
            get
            {
                return errorStatus;
            }
            set
            {
                errorStatus = $"Error: {value}";
                OnPropertyChanged(nameof(ErrorStatus));
            }
        }

        private void InitialUpdate()
        {
            var position = new byte[5];
            if (motorModel.GetMotorPosition(ref position))
            {
                MotorPosition = Helper.GetIntFromBigEndian(position).ToString();
            }

            var status = new byte[5];

            if (motorModel.GetMotorStatus(ref status))
            {
                ProcessMotorStatus(status);
            }
        }

        private void RefreshPosition()
        {
            var position = new byte[5];
            if (motorModel.GetMotorPosition(ref position))
            {
                MotorPosition = Helper.GetIntFromBigEndian(position).ToString();
            }
        }

        private void RefreshMotorStatus()
        {
            var status = new byte[5];

            if (motorModel.GetMotorStatus(ref status))
            {
                ProcessMotorStatus(status);
            }
        }

        private void ProcessMotorStatus(byte[] status)
        {
            if (Helper.IsBitSet(status[0], 0))
            {
                BridgeInHiz = "Yes";
            }
            else
            {
                BridgeInHiz = "No";
            }

            if (Helper.IsBitSet(status[0], 1))
            {
                BusyStatus = "Yes";
            }
            else
            {
                BusyStatus = "No";
            }

            if (Helper.IsBitSet(status[1], 0))
            {
                ErrorStatus = "Yes";
            }
            else
            {
                ErrorStatus = "No";
            }
        }

        private void Energize()
        {
            var response = new byte[5];
            motorModel.Energize(ref response);
        }

        private async void SetRegister()
        {
            var response = new byte[5];
            motorModel.SetRegisterValue(registerValue, ref response);
        }

        private async void MoveToRelativePosition()
        {
            var response = new byte[5];
            motorModel.MotorControlMove(selectedDirection, selectedStepSize, "relative", ref response);
        }

        private async void MoveToAbsolutePosition()
        {
            var response = new byte[5];
            motorModel.MotorControlMove(selectedDirection, selectedStepSize, "absolute", ref response);
        }

        private void SetRelativeMovePosition()
        {
            var response = new byte[5];
            motorModel.SetRelativeMovePosition(relativeMoveValue, ref response);
        }

        private void SetAbsoluteMovePosition()
        {
            var response = new byte[5];
            motorModel.SetAbsoluteMovePosition(absoluteMoveValue, ref response);
        }

        private async void SetSelectedRegisterAddress()
        {
            var response = new byte[5];
            motorModel.SetRegisterAddress(selectedRegisterAddress, ref response);
        }

        //private async void SetConfiguration()
        //{
        //    await motorModel.SetDirection(selectedDirection);
        //    await motorModel.SetStepMode(selectedStepSize);
        //}

        //private void SetStepMode()
        //{
        //    motorModel.SetStepMode(selectedStepSize).Wait();
        //}

        //private void SetDirectionMode()
        //{
        //    motorModel.SetDirection(selectedDirection).Wait();
        //}

        private async void ReadRegister()
        {
            // Read register address
            var regValue = new byte[5];
            if (motorModel.ReadRegisterValue(ref regValue))
            {
                RegisterReadValue = Helper.GetIntFromBigEndian(regValue).ToString();
            }
        }

        private async void Initialize()
        {
            var response = new byte[5];
            motorModel.Initialize(ref response);
        }

        private async void Limp()
        {
            var response = new byte[5];
            motorModel.Limp(ref response);
        }

        private async void Reset()
        {
            var response = new byte[5];
            motorModel.Reset(ref response);
        }

        private void Home()
        {
            var response = new byte[5];
            motorModel.Home(ref response);

            //motorModel.GetMotorPosition();
        }

        private void Cycle()
        {
            var response = new byte[5];
            motorModel.Cycle(ref response);
        }

        private async void Stop()
        {
            var response = new byte[5];
            motorModel.Stop(ref response);
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
                    // Read motor position
                    var position = new byte[5];
                    if (motorModel.GetMotorPosition(ref position))
                    {
                        MotorPosition = Helper.GetIntFromBigEndian(position).ToString();
                    }
                    
                    Thread.Sleep(50);

                    // Update motor status
                    InitialUpdate();
                }
                catch (Exception)
                {
                   
                }
            }
        }
    }
}
