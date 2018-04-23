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
        public int absoluteMoveValue;
        public int relativeMoveValue;
        private byte[] registerValue;

        private const int updateDelay = 300;
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
            absoluteMoveValue = 0;
            relativeMoveValue = 0;

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

            StartUpdateTask();
        }



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

        private void InitialUpdate()
        {
            //motorModel.InitialSet();
            SelectedDirection = Directions[0];
            SelectedStepSize = StepSizes[0];

            var position = motorModel.GetMotorPosition().Result;
            MotorPosition = Helper.GetIntFromBigEndian(position).ToString();

            //motorModel.SetRegisterAddress(selectedRegisterAddress).Wait();
            //RegisterReadValue = Helper.GetFloatFromBigEndian(motorModel.ReadRegisterValue().Result).ToString();
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
            await motorModel.MotorControlMove(selectedDirection, selectedStepSize, "relative");
        }

        private void SetRelativeMovePosition()
        {
            motorModel.SetRelativeMovePosition(relativeMoveValue);
        }

        private void SetAbsoluteMovePosition()
        {
            motorModel.SetRelativeMovePosition(absoluteMoveValue);
        }

        private async void SetSelectedRegisterAddress()
        {
            await motorModel.SetRegisterAddress(selectedRegisterAddress);
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
                    // Read register address
                    var registerValue = await motorModel.ReadRegisterValue();
                    RegisterReadValue = Helper.GetIntFromBigEndian(registerValue).ToString();

                    // Read motor position
                    var position = await motorModel.GetMotorPosition();
                    MotorPosition = Helper.GetIntFromBigEndian(position).ToString();

                    // Update motor status
                    Thread.Sleep(updateDelay);
                }
                catch (Exception e)
                {
                    //StatusMessage = e.Message;
                }
            }
        }
    }
}
