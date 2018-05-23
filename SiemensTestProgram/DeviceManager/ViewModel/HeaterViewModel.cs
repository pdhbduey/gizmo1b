using Common.Bindings;
using DeviceManager.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.ViewModel
{
    public class HeaterViewModel : BindableBase
    {
        IHeaterModel heaterModel;

        private float proportionalGain;
        private float integralGain;
        private float derivativeGain;
        private float tRef;
        private float iMax;

        public HeaterViewModel(IHeaterModel heaterModel)
        {
            this.heaterModel = heaterModel;

            SetIntegralGainCommand = new RelayCommand(param => UpdateIntegralGain());
            SetProportionalGainCommand = new RelayCommand(param => UpdateProportionalGain());
            SetDerivativeGainCommand = new RelayCommand(param => UpdateDerivativeGain());
            SetTRefCommand = new RelayCommand(param => UpdateTRef());
            SetIMaxCommand = new RelayCommand(param => UpdateIMax());

            // InitialUpdate

            // UpdateTask
        }

        public float ProportionalGain
        {
            get
            {
                return proportionalGain;
            }
            set
            {
                proportionalGain = value;

                OnPropertyChanged(nameof(ProportionalGain));
                //UpdateProportionalGain();
            }
        }

        public float IntegralGain
        {
            get
            {
                return integralGain;
            }
            set
            {
                integralGain = value;

                OnPropertyChanged(nameof(IntegralGain));
                //UpdateIntegralGain();
            }
        }

        public float DerivativeGain
        {
            get
            {
                return derivativeGain;
            }
            set
            {
                derivativeGain = value;

                OnPropertyChanged(nameof(DerivativeGain));
                //UpdateDerivativeGain();
            }
        }

        public float IMax
        {
            get
            {
                return iMax;
            }
            set
            {
                iMax = value;

                OnPropertyChanged(nameof(IMax));
                //UpdateIMax();
            }
        }

        public float TRef
        {
            get
            {
                return tRef;
            }
            set
            {
                tRef = value;

                OnPropertyChanged(nameof(TRef));
                //UpdateTRef();
            }
        }

        public RelayCommand SetIntegralGainCommand { get; set; }

        public RelayCommand SetProportionalGainCommand { get; set; }

        public RelayCommand SetDerivativeGainCommand { get; set; }

        public RelayCommand SetTRefCommand { get; set; }

        public RelayCommand SetIMaxCommand { get; set; }
        

        private async void UpdateProportionalGain()
        {
            if (proportionalGain < HeaterDefaults.ProportionalGainMinimum)
            {
                ProportionalGain = HeaterDefaults.ProportionalGainMinimum;
            }
            else if (proportionalGain > HeaterDefaults.ProportionalGainMaximum)
            {
                ProportionalGain = HeaterDefaults.ProportionalGainMaximum;
            }

            var response = new byte[5];
            await heaterModel.SetProportionalGainCommand(proportionalGain);
        }

        private async void UpdateIntegralGain()
        {
            if (integralGain < HeaterDefaults.IntegralGainMinimum)
            {
                IntegralGain = HeaterDefaults.IntegralGainMinimum;
            }
            else if (integralGain > HeaterDefaults.IntegralGainMaximum)
            {
                IntegralGain = HeaterDefaults.IntegralGainMaximum;
            }

            var response = new byte[5];
            await heaterModel.SetIntegralGainCommand(integralGain);
        }

        private async void UpdateDerivativeGain()
        {
            if (derivativeGain < HeaterDefaults.DerivativeGainMinimum)
            {
                DerivativeGain = HeaterDefaults.DerivativeGainMinimum;
            }
            else if (derivativeGain > HeaterDefaults.DerivativeGainMaximum)
            {
                DerivativeGain = HeaterDefaults.DerivativeGainMaximum;
            }

            var response = new byte[5];
            await heaterModel.SetDerivativeGainCommand(derivativeGain);
        }

        private async void UpdateTRef()
        {
            if (tRef < HeaterDefaults.TRefMinimum)
            {
                TRef = HeaterDefaults.TRefMinimum;
            }
            else if (tRef > HeaterDefaults.TRefMaximum)
            {
                TRef = HeaterDefaults.TRefMaximum;
            }

            var response = new byte[5];
            await heaterModel.SetTRefCommand(tRef);
        }

        private async void UpdateIMax()
        {
            if (iMax < HeaterDefaults.IMaxMinimum)
            {
                IMax = HeaterDefaults.IMaxMinimum;
            }
            else if (iMax > HeaterDefaults.IMaxMaximum)
            {
                IMax = HeaterDefaults.IMaxMaximum;
            }

            var response = new byte[5];
            await heaterModel.SetIMaxCommand(iMax);
        }
    }
}
