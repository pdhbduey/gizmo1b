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
    using System.Windows;

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

        private const string notActiveColour = "Gray";
        private const string errorColour = "Red";
        private const string statusYesColour = "Green";
        private const int updateDelay = 400;

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

            StartUpdateTask();
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

        private async void InitialUpdate()
        {
            var position = await motorModel.GetMotorPosition();
            if (position.succesfulResponse)
            {
                MotorPosition = Helper.GetIntFromBigEndian(position.response).ToString();
            }

            var status = await motorModel.GetMotorStatus();
            if (status.succesfulResponse)
            {
                ProcessMotorStatus(status.response);
            }
        }

        private async void RefreshPosition()
        {
            var position = await motorModel.GetMotorPosition();
            if (position.succesfulResponse)
            {
                MotorPosition = Helper.GetIntFromBigEndian(position.response).ToString();
            }
        }

        private async void RefreshMotorStatus()
        {
            var status = await motorModel.GetMotorStatus();

            if (status.succesfulResponse)
            {
                ProcessMotorStatus(status.response);
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
                BridgeStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[4], 1))
            {
                BusyStatus = statusYesColour;
            }
            else
            {
                BusyStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[4], 2))
            {
                SwitchClosedStatus = statusYesColour;
            }
            else
            {
                SwitchClosedStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[4], 3))
            {
                SwitchOpenStatus = statusYesColour;
            }
            else
            {
                SwitchOpenStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[4], 4))
            {
                DirectionForwardStatus = statusYesColour;
            }
            else
            {
                DirectionForwardStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[4], 5))
            {
                DirectionReverseStatus = statusYesColour;
            }
            else
            {
                DirectionReverseStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[4], 6))
            {
                StoppedStatus = statusYesColour;
            }
            else
            {
                StoppedStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[4], 7))
            {
                AcceleratingStatus = statusYesColour;
            }
            else
            {
                AcceleratingStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[3], 0))
            {
                DeceleratingStatus = statusYesColour;
            }
            else
            {
                DeceleratingStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[3], 1))
            {
                RunAtConstantSpeedStatus = statusYesColour;
            }
            else
            {
                RunAtConstantSpeedStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[3], 2))
            {
                ThermalWarningStatus = errorColour;
            }
            else
            {
                ThermalWarningStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[3], 3))
            {
                ThermalShutdownStatus = errorColour;
            }
            else
            {
                ThermalShutdownStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[3], 4))
            {
                OvercurrentStatus = errorColour;
            }
            else
            {
                OvercurrentStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[3], 5))
            {
                BridgeAStatus = errorColour;
            }
            else
            {
                BridgeAStatus = notActiveColour;
            }

            if (Helper.IsBitSet(status[3], 6))
            {
                BridgeBStatus = errorColour;
            }
            else
            {
                BridgeBStatus = notActiveColour;
            }
        }

        private async void Energize()
        {
            await motorModel.Energize();
        }

        private async void SetRegister()
        {
            await motorModel.SetRegisterValue(registerValue);
        }

        private async void MoveToRelativePosition()
        {
            await motorModel.MotorControlMove(selectedDirection, selectedStepSize, "relative");
        }

        private async void MoveToAbsolutePosition()
        {
            await motorModel.MotorControlMove(selectedDirection, selectedStepSize, "absolute");
        }

        private async void SetRelativeMovePosition()
        {
            await motorModel.SetRelativeMovePosition(relativeMoveValue);
        }

        private async void SetAbsoluteMovePosition()
        {
            await motorModel.SetAbsoluteMovePosition(absoluteMoveValue);
        }

        private async void SetSelectedRegisterAddress()
        {
            await motorModel.SetRegisterAddress(selectedRegisterAddress);
        }

        private async void ReadRegister()
        {
            // Read register address
            var regValue = await motorModel.ReadRegisterValue();
            if (regValue.succesfulResponse)
            {
                RegisterReadValue = Helper.GetIntFromBigEndian(regValue.response).ToString();
            }
        }

        private async void Initialize()
        {
            await motorModel.Initialize();
        }

        private async void Limp()
        {
            await motorModel.Limp();
        }

        private async void Reset()
        {
            await motorModel.Reset();
        }

        private async void Home()
        {
            var response = new byte[5];
            await motorModel.Home();
        }

        private async void Cycle()
        {
            await motorModel.Cycle();
        }

        private async void Stop()
        {
            await motorModel.Stop();
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
                var position = await motorModel.GetMotorPosition();
                if (position.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        MotorPosition = Helper.GetIntFromBigEndian(position.response).ToString();
                    }));
                    
                }
                Thread.Sleep(updateDelay);

                var status = await motorModel.GetMotorStatus();
                if (status.succesfulResponse)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        ProcessMotorStatus(status.response);
                    }));
                    
                }
                Thread.Sleep(updateDelay);
            }
        }
    }
}
