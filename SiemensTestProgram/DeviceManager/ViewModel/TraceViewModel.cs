using Common;
using Common.Bindings;
using DeviceManager.DeviceCommunication;
using DeviceManager.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

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
            VSenseCollection.Clear();
            ISenseCollection.Clear();
            IRefCollection.Clear();
            TemperatureFourCollection.Clear();
            TemperatureThreeCollection.Clear();
            TemperatureTwoCollection.Clear();
            TemperatureOneCollection.Clear();

            cts = new CancellationTokenSource();
            token = cts.Token;

            var task = Task.Factory.StartNew(() =>
            {
                TraceSamples();
            }, token);
        }

        private async void StopTrace()
        {
            cts.Cancel();
            traceModel.StopTrace().Wait();
        }

        private async void TraceSamples()
        {
            var modValue = 1000;
            var samplesToClear = 0;
            double sampledTime = 0;

            while(token.IsCancellationRequested == false)
            {
                var numberOfAvailableSamples = traceModel.GetNumberOfAvailableSamples().Result;
                if (numberOfAvailableSamples.succesfulResponse)
                {
                    var availableSamples = Helper.GetIntFromLittleEndian(numberOfAvailableSamples.response);
                    var firstAvailableSample = traceModel.GetFirstAvailableSampleIndex().Result;
                    if (firstAvailableSample.succesfulResponse)
                    {
                        var i = Helper.GetIntFromLittleEndian(firstAvailableSample.response) % modValue;
                        
                        for (var j = 0; j < availableSamples; j++)
                        {
                            sampledTime += 0.1;

                            // Update chart
                            var vSense = traceModel.ReadVsenseSamples(i).Result;
                            if (vSense.succesfulResponse)
                            {
                                var vSenseDataValue = Helper.GetFloatFromBigEndian(vSense.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    VSenseCollection.Add(new DataPoint(sampledTime, vSenseDataValue));
                                }));
                            }

                            var iSense = traceModel.ReadIsenseSamples(i).Result;
                            if (iSense.succesfulResponse)
                            {
                                var iSenseDataValue = Helper.GetFloatFromBigEndian(iSense.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    ISenseCollection.Add(new DataPoint(sampledTime, iSenseDataValue));
                                }));
                            }

                            var iRef = traceModel.ReadIrefSamples(i).Result;
                            if (iRef.succesfulResponse)
                            {
                                var iRefDataValue = Helper.GetFloatFromBigEndian(iRef.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    IRefCollection.Add(new DataPoint(sampledTime, iRefDataValue));
                                }));
                            }

                            var temperatureOne = traceModel.ReadTempOneSamples(i).Result;
                            if (temperatureOne.succesfulResponse)
                            {
                                var temperatureOneDataValue = Helper.GetFloatFromBigEndian(temperatureOne.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    TemperatureOneCollection.Add(new DataPoint(sampledTime, temperatureOneDataValue));
                                }));
                            }

                            var temperatureTwo = traceModel.ReadTempTwoSamples(i).Result;
                            if (temperatureTwo.succesfulResponse)
                            {
                                var temperatureTwoDataValue = Helper.GetFloatFromBigEndian(temperatureTwo.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    TemperatureTwoCollection.Add(new DataPoint(sampledTime, temperatureTwoDataValue));
                                }));
                            }

                            var temperatureThree = traceModel.ReadTempThreeSamples(i).Result;
                            if (temperatureThree.succesfulResponse)
                            {
                                var temperatureThreeDataValue = Helper.GetFloatFromBigEndian(temperatureThree.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    TemperatureThreeCollection.Add(new DataPoint(sampledTime, temperatureThreeDataValue));
                                }));
                            }

                            var temperatureFour = traceModel.ReadTempFourSamples(i).Result;
                            if (temperatureFour.succesfulResponse)
                            {
                                var temperatureFourDataValue = Helper.GetFloatFromBigEndian(temperatureFour.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    TemperatureFourCollection.Add(new DataPoint(sampledTime, temperatureFourDataValue));
                                }));
                            }

                            // update what to read next
                            i = (i + 1) % modValue;
                            samplesToClear++;

                            var removeSamples = traceModel.SetReadSamples(samplesToClear).Result;
                            if (removeSamples.succesfulResponse)
                            {
                                samplesToClear = 0;
                            }
                        }
                    }
                }

            }
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
