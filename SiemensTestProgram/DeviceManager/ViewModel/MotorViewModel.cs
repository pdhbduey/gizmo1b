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
        private string bridgeStatus;
        private string busyStatus;
        private string switchClosedStatus;
        private string switchOpenStatus;
        private string directionForwardStatus;
        private string directionReverseStatus;
        private string stoppedStatus;
        private string acceleratingStatus;
        private string deceleratingStatus;
        private string runAtConstantSpeedStatus;
        private string thermalWarningStatus;
        private string thermalShutdownStatus;
        private string overcurrentStatus;
        private string bridgeAStatus;
        private string bridgeBStatus;

        private int absoluteMoveValue;
        private int relativeMoveValue;
        private byte[] registerValue;

        private const string errorColour = "Red";
        private const string noErrorColour = "Green";
        private const string statusYesColour = "Green";
        private const string statusNoColour = "Red";
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

        public string BridgeStatus
        {
            get
            {
                return bridgeStatus;
            }
            set
            {
                bridgeStatus = value;
                OnPropertyChanged(nameof(BridgeStatus));
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
                busyStatus = value;
                OnPropertyChanged(nameof(BusyStatus));
            }
        }

        public string SwitchClosedStatus
        {
            get
            {
                return switchClosedStatus;
            }
            set
            {
                switchClosedStatus = value;
                OnPropertyChanged(nameof(SwitchClosedStatus));
            }
        }
        public string SwitchOpenStatus
        {
            get
            {
                return switchOpenStatus;
            }
            set
            {
                switchOpenStatus = value;
                OnPropertyChanged(nameof(SwitchOpenStatus));
            }
        }
        public string DirectionForwardStatus
        {
            get
            {
                return directionForwardStatus;
            }
            set
            {
                directionForwardStatus = value;
                OnPropertyChanged(nameof(DirectionForwardStatus));
            }
        }
        public string DirectionReverseStatus
        {
            get
            {
                return directionReverseStatus;
            }
            set
            {
                directionReverseStatus = value;
                OnPropertyChanged(nameof(DirectionReverseStatus));
            }
        }
        public string StoppedStatus
        {
            get
            {
                return stoppedStatus;
            }
            set
            {
                stoppedStatus = value;
                OnPropertyChanged(nameof(StoppedStatus));
            }
        }
        public string AcceleratingStatus
        {
            get
            {
                return acceleratingStatus;
            }
            set
            {
                acceleratingStatus = value;
                OnPropertyChanged(nameof(AcceleratingStatus));
            }
        }
        public string DeceleratingStatus
        {
            get
            {
                return deceleratingStatus;
            }
            set
            {
                deceleratingStatus = value;
                OnPropertyChanged(nameof(DeceleratingStatus));
            }
        }
        public string RunAtConstantSpeedStatus
        {
            get
            {
                return runAtConstantSpeedStatus;
            }
            set
            {
                runAtConstantSpeedStatus = value;
                OnPropertyChanged(nameof(RunAtConstantSpeedStatus));
            }
        }
        public string ThermalWarningStatus
        {
            get
            {
                return thermalWarningStatus;
            }
            set
            {
                thermalWarningStatus = value;
                OnPropertyChanged(nameof(ThermalWarningStatus));
            }
        }
        public string ThermalShutdownStatus
        {
            get
            {
                return thermalShutdownStatus;
            }
            set
            {
                thermalShutdownStatus = value;
                OnPropertyChanged(nameof(ThermalShutdownStatus));
            }
        }

        public string OvercurrentStatus
        {
            get
            {
                return overcurrentStatus;
            }
            set
            {
                overcurrentStatus = value;
                OnPropertyChanged(nameof(OvercurrentStatus));
            }
        }

        public string BridgeAStatus
        {
            get
            {
                return bridgeAStatus;
            }
            set
            {
                bridgeAStatus = value;
                OnPropertyChanged(nameof(BridgeAStatus));
            }
        }

        public string BridgeBStatus
        {
            get
            {
                return bridgeBStatus;
            }
            set
            {
                bridgeBStatus = value;
                OnPropertyChanged(nameof(BridgeBStatus));
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
            if (Helper.IsBitSet(status[4], 0))
            {
                BridgeStatus = statusYesColour;
            }
            else
            {
                BridgeStatus = statusNoColour;
            }

            if (Helper.IsBitSet(status[4], 1))
            {
                BusyStatus = errorColour;
            }
            else
            {
                BusyStatus = noErrorColour;
            }

            if (Helper.IsBitSet(status[4], 2))
            {
                SwitchClosedStatus = statusYesColour;
            }
            else
            {
                SwitchClosedStatus = statusNoColour;
            }

            if (Helper.IsBitSet(status[4], 3))
            {
                SwitchOpenStatus = statusYesColour;
            }
            else
            {
                SwitchOpenStatus = statusNoColour;
            }

            if (Helper.IsBitSet(status[4], 4))
            {
                DirectionForwardStatus = statusYesColour;
            }
            else
            {
                DirectionForwardStatus = statusNoColour;
            }

            if (Helper.IsBitSet(status[4], 5))
            {
                DirectionReverseStatus = statusYesColour;
            }
            else
            {
                DirectionReverseStatus = statusNoColour;
            }

            if (Helper.IsBitSet(status[4], 6))
            {
                StoppedStatus = statusYesColour;
            }
            else
            {
                StoppedStatus = statusNoColour;
            }

            if (Helper.IsBitSet(status[4], 7))
            {
                AcceleratingStatus = statusYesColour;
            }
            else
            {
                AcceleratingStatus = statusNoColour;
            }

            if (Helper.IsBitSet(status[3], 0))
            {
                DeceleratingStatus = statusYesColour;
            }
            else
            {
                DeceleratingStatus = statusNoColour;
            }

            if (Helper.IsBitSet(status[3], 1))
            {
                RunAtConstantSpeedStatus = statusYesColour;
            }
            else
            {
                RunAtConstantSpeedStatus = statusNoColour;
            }

            if (Helper.IsBitSet(status[3], 2))
            {
                ThermalWarningStatus = errorColour;
            }
            else
            {
                ThermalWarningStatus = noErrorColour;
            }

            if (Helper.IsBitSet(status[3], 3))
            {
                ThermalShutdownStatus = errorColour;
            }
            else
            {
                ThermalShutdownStatus = noErrorColour;
            }

            if (Helper.IsBitSet(status[3], 4))
            {
                OvercurrentStatus = errorColour;
            }
            else
            {
                OvercurrentStatus = noErrorColour;
            }

            if (Helper.IsBitSet(status[3], 5))
            {
                BridgeAStatus = errorColour;
            }
            else
            {
                BridgeAStatus = noErrorColour;
            }

            if (Helper.IsBitSet(status[3], 6))
            {
                BridgeBStatus = errorColour;
            }
            else
            {
                BridgeBStatus = noErrorColour;
            }
        }

        private void Energize()
        {
            var response = new byte[5];
            motorModel.Energize(ref response);
        }

        private void SetRegister()
        {
            var response = new byte[5];
            motorModel.SetRegisterValue(registerValue, ref response);
        }

        private void MoveToRelativePosition()
        {
            var response = new byte[5];
            motorModel.MotorControlMove(selectedDirection, selectedStepSize, "relative", ref response);
        }

        private void MoveToAbsolutePosition()
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

        private void SetSelectedRegisterAddress()
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

        private void ReadRegister()
        {
            // Read register address
            var regValue = new byte[5];
            if (motorModel.ReadRegisterValue(ref regValue))
            {
                RegisterReadValue = Helper.GetIntFromBigEndian(regValue).ToString();
            }
        }

        private void Initialize()
        {
            var response = new byte[5];
            motorModel.Initialize(ref response);
        }

        private void Limp()
        {
            var response = new byte[5];
            motorModel.Limp(ref response);
        }

        private void Reset()
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

        private void Stop()
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

        private void UpdateAllStatuses()
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
