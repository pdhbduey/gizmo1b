// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using Common.Bindings;
    using DeviceManager.Model;
    using System;
    using System.Collections.Generic;

    public class MotorViewModel : BindableBase
    {
        private IMotorModel motorModel;
        private string selectedStepSize;
        private string selectedDirection;

        public MotorViewModel(IMotorModel motorModel)
        {
            this.motorModel = motorModel;

            // initial values
            StepSizes = MotorDefaults.StepSizes;
            selectedStepSize = StepSizes[0];

            Directions = MotorDefaults.Directions;
            selectedDirection = Directions[0];

            SetConfigurationCommand = new RelayCommand(param => SetConfiguration());
            HomeCommand = new RelayCommand(param => Home());
            ResetCommand = new RelayCommand(param => Reset());
            LimpCommand = new RelayCommand(param => Limp());
            InitializeCommand = new RelayCommand(param => Initialize());
        }

        public RelayCommand SetConfigurationCommand { get; set; }

        public RelayCommand ResetCommand { get; set; }

        public RelayCommand LimpCommand { get; set; }

        public RelayCommand HomeCommand { get; set; }

        public RelayCommand InitializeCommand { get; set; }

        public List<string> StepSizes { get; set; }

        public List<string> Directions { get; set; }

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

        private async void SetConfiguration()
        {
            await motorModel.SetDirection(selectedDirection);
            await motorModel.SetStepMode(selectedStepSize);
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
            await motorModel.Home();
        }
    }
}
