using Common.Bindings;
using DeviceManager.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DeviceManager.ViewModel
{
    public class TraceViewModel : BindableBase
    {
        private ITraceModel traceModel;
        private int selectedResolution;
        private int numberOfSamples;

        private CancellationTokenSource cts;
        private CancellationToken token;
        private Task saveTask;

        public TraceViewModel(ITraceModel traceModel)
        {
            this.traceModel = traceModel;

            VSenseCollection = new ObservableCollection<DataPoint>();
            IRefCollection = new ObservableCollection<DataPoint>();
            ISenseCollection = new ObservableCollection<DataPoint>();
            TemperatureOneCollection = new ObservableCollection<DataPoint>();
            TemperatureTwoCollection = new ObservableCollection<DataPoint>();
            TemperatureThreeCollection = new ObservableCollection<DataPoint>();
            TemperatureFourCollection = new ObservableCollection<DataPoint>();

            Resolutions = TraceDefaults.Resolutions;
            SelectedResolution = Resolutions[0];

            NumberOfSamples = TraceDefaults.SampleNumberMinimum;

            StartTraceCommand = new RelayCommand(param => StartTrace());
            StopTraceCommand = new RelayCommand(param => StopTrace());
            SetNumberOfSamplesCommand = new RelayCommand(param => SetNumberOfSamples());
        }

        public List<int> Resolutions { get; set; }

        public int SelectedResolution
        {
            get
            {
                return selectedResolution;
            }

            set
            {
                selectedResolution = value;
                OnPropertyChanged(nameof(SelectedResolution));

                SetResolution();
            }
        }

        public int NumberOfSamples
        {
            get
            {
                return numberOfSamples;
            }

            set
            {
                numberOfSamples = value;
                OnPropertyChanged(nameof(NumberOfSamples));
            }
        }

        private async void SetResolution()
        {
            await traceModel.SetResolution(selectedResolution);
        }

        private async void StartTrace()
        {
            traceModel.StartTrace().Wait();
        }

        private async void StopTrace()
        {
            await traceModel.StopTrace();
        }

        private async void SetNumberOfSamples()
        {
            if (numberOfSamples > TraceDefaults.SampleNumberMaximum)
            {
                NumberOfSamples = TraceDefaults.SampleNumberMaximum;
            }
            else if (numberOfSamples < TraceDefaults.SampleNumberMinimum)
            {
                NumberOfSamples = TraceDefaults.SampleNumberMinimum;
            }

            await traceModel.SetNumberOfSamples(numberOfSamples);
        }

        public RelayCommand StartTraceCommand { get; set; }

        public RelayCommand StopTraceCommand { get; set; }

        public RelayCommand SetNumberOfSamplesCommand { get; set; }

        public class DataPoint
        {
            public DataPoint(double sample, double value)
            {
                Sample = sample;
                Value = value;
            }

            public double Value { get; set; }
            public double Sample { get; set; }
        }

        public ObservableCollection<DataPoint> VSenseCollection { get; set; }

        public ObservableCollection<DataPoint> ISenseCollection { get; set; }

        public ObservableCollection<DataPoint> IRefCollection { get; set; }

        public ObservableCollection<DataPoint> TemperatureOneCollection { get; set; }

        public ObservableCollection<DataPoint> TemperatureTwoCollection { get; set; }

        public ObservableCollection<DataPoint> TemperatureThreeCollection { get; set; }

        public ObservableCollection<DataPoint> TemperatureFourCollection { get; set; }

    }
}
