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
using System.Windows.Controls.DataVisualization.Charting;

namespace DeviceManager.ViewModel
{
    public class TraceViewModel : BindableBase
    {
        private ITraceModel traceModel;
        private int selectedResolution;
        private int numberOfSamples;
        private const int resolutionRangeMultiplier = 2;

        private int refresh;
        private double refreshTime;

        private double sampleMinimumX;
        private double sampleMaximumX;
        
        private CancellationTokenSource cts;
        private CancellationToken token;
        private Task saveTask;

        public TraceViewModel(ITraceModel traceModel)
        {
            this.traceModel = traceModel;
            Resolutions = TraceDefaults.Resolutions;
            SelectedResolution = Resolutions[0];

            VSenseCollection = new BulkObservableCollection<DataPoint>(refresh * 2);
            IRefCollection = new BulkObservableCollection<DataPoint>(refresh * 2);
            ISenseCollection = new BulkObservableCollection<DataPoint>(refresh * 2);
            TemperatureOneCollection = new BulkObservableCollection<DataPoint>(refresh * 2);
            TemperatureTwoCollection = new BulkObservableCollection<DataPoint>(refresh * 2);
            TemperatureThreeCollection = new BulkObservableCollection<DataPoint>(refresh * 2);
            TemperatureFourCollection = new BulkObservableCollection<DataPoint>(refresh * 2);

            NumberOfSamples = TraceDefaults.SampleNumberMinimum;
            SampleMaximumX = 3;
            SampleMinimumX = 0;

            StartTraceCommand = new RelayCommand(param => StartTrace());
            StopTraceCommand = new RelayCommand(param => StopTrace());
            SetNumberOfSamplesCommand = new RelayCommand(param => SetNumberOfSamples());
        }

        public double SampleMinimumX
        {
            get
            {
                return sampleMinimumX;
            }

            set
            {
                sampleMinimumX = value;
                OnPropertyChanged(nameof(SampleMinimumX));
            }
        }

        public double SampleMaximumX
        {
            get
            {
                return sampleMaximumX;
            }

            set
            {
                sampleMaximumX = value;
                OnPropertyChanged(nameof(SampleMaximumX));
            }
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

        private void SetResolution()
        {
            traceModel.SetResolution(selectedResolution).Wait();
            if (selectedResolution == 10)
            {
                refresh = 10;
                refreshTime = 1.0;
            }
            else
            {
                refresh = 200;
                refreshTime = 2.0;
            }
            //VSenseCollection.UpdateThreshold = selectedResolution;
            //IRefCollection.UpdateThreshold = selectedResolution;
            //ISenseCollection.UpdateThreshold = selectedResolution;
            //TemperatureOneCollection.UpdateThreshold = selectedResolution;
            //TemperatureTwoCollection.UpdateThreshold = selectedResolution;
            //TemperatureThreeCollection.UpdateThreshold = selectedResolution;
            //TemperatureFourCollection.UpdateThreshold = selectedResolution;
        }

        private async void StartTrace()
        {
            traceModel.StopTrace().Wait();
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
            cts?.Cancel();
            traceModel.StopTrace().Wait();
        }

        private async void TraceSamples()
        {
            var modValue = 1000;
            var samplesToClear = 0;
            double sampledTime = 0;
            double sampleNumber = 0;
            var updateRanges = 0;
            var m = 10;

            while (token.IsCancellationRequested == false)
            {
                var numberOfAvailableSamples = traceModel.GetNumberOfAvailableSamples().Result;
                if (numberOfAvailableSamples.succesfulResponse)
                {
                    var availableSamples = Helper.GetIntFromBigEndian(numberOfAvailableSamples.response);
                    var firstAvailableSample = traceModel.GetFirstAvailableSampleIndex().Result;
                    if (firstAvailableSample.succesfulResponse)
                    {
                        var i = Helper.GetIntFromBigEndian(firstAvailableSample.response) % modValue;

                        //for (var t = 0; t < availableSamples; t+=m)
                        //{
                        //    if (token.IsCancellationRequested == true)
                        //    {
                        //        break;
                        //    }
                        
                            for (var j = 0; j < Math.Min(m, availableSamples); j++)
                            {
                                sampledTime = sampleNumber / (double)selectedResolution;

                            // Update chart
                            //var vSense = traceModel.ReadVsenseSamples(i).Result;
                            //if (vSense.succesfulResponse)
                            //{
                            //    var vSenseDataValue = Helper.GetFloatFromBigEndian(vSense.response);

                            //    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                            //    {
                            //        VSenseCollection.SurpressedAdd(new DataPoint(sampledTime, vSenseDataValue));

                            //        if (sampleNumber > resolutionRangeMultiplier * selectedResolution)
                            //        {
                            //            VSenseCollection.SurpressedRemoveAt(0);
                            //        }
                            //    }));
                            //}

                            //var iSense = traceModel.ReadIsenseSamples(i).Result;
                            //if (iSense.succesfulResponse)
                            //{
                            //    var iSenseDataValue = Helper.GetFloatFromBigEndian(iSense.response);

                            //    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                            //    {
                            //        ISenseCollection.SurpressedAdd(new DataPoint(sampledTime, iSenseDataValue));

                            //        if (sampleNumber > resolutionRangeMultiplier * selectedResolution)
                            //        {
                            //            ISenseCollection.SurpressedRemoveAt(0);
                            //        }
                            //    }));
                            //}

                            //var iRef = traceModel.ReadIrefSamples(i).Result;
                            //if (iRef.succesfulResponse)
                            //{
                            //    var iRefDataValue = Helper.GetFloatFromBigEndian(iRef.response);

                            //    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                            //    {
                            //        IRefCollection.SurpressedAdd(new DataPoint(sampledTime, iRefDataValue));

                            //        if (sampleNumber > resolutionRangeMultiplier * selectedResolution)
                            //        {
                            //            IRefCollection.SurpressedRemoveAt(0);
                            //        }
                            //    }));
                            //}

                            //var temperatureOne = traceModel.ReadTempOneSamples(i).Result;
                            //if (temperatureOne.succesfulResponse)
                            //{
                            //    var temperatureOneDataValue = Helper.GetFloatFromBigEndian(temperatureOne.response);

                            //    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                            //    {
                            //        TemperatureOneCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureOneDataValue));

                            //        if (sampleNumber > resolutionRangeMultiplier * selectedResolution)
                            //        {
                            //            TemperatureOneCollection.SurpressedRemoveAt(0);
                            //        }
                            //    }));
                            //}

                            //var temperatureTwo = traceModel.ReadTempTwoSamples(i).Result;
                            //if (temperatureTwo.succesfulResponse)
                            //{
                            //    var temperatureTwoDataValue = Helper.GetFloatFromBigEndian(temperatureTwo.response);

                            //    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                            //    {
                            //        TemperatureTwoCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureTwoDataValue));

                            //        if (sampleNumber > resolutionRangeMultiplier * selectedResolution)
                            //        {
                            //            TemperatureTwoCollection.SurpressedRemoveAt(0);
                            //        }
                            //    }));
                            //}

                            var temperatureThree = traceModel.ReadTempThreeSamples(i).Result;
                            if (temperatureThree.succesfulResponse)
                            {
                                var temperatureThreeDataValue = Helper.GetFloatFromBigEndian(temperatureThree.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    TemperatureThreeCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureThreeDataValue));

                                    if (sampleNumber >  selectedResolution)
                                    {
                                        TemperatureThreeCollection.SurpressedRemoveAt(0);
                                    }
                                }));
                            }

                            var temperatureFour = traceModel.ReadTempFourSamples(i).Result;
                                    if (temperatureFour.succesfulResponse)
                                    {
                                        var temperatureFourDataValue = Helper.GetFloatFromBigEndian(temperatureFour.response);
                                
                                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                        {
                                            TemperatureFourCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureFourDataValue));
                                            if (sampleNumber >  selectedResolution)
                                            {
                                                TemperatureFourCollection.SurpressedRemoveAt(0);
                                            }
                                            
                                        }));
                                }
                                // update what to read next
                                i = (i + 1) % modValue;
                                samplesToClear++;
                                sampleNumber++;
                                updateRanges++;


                            if (updateRanges >= refresh)
                            {
                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    SampleMaximumX = sampledTime;
                                    SampleMinimumX = sampledTime - refreshTime;
                                }));

                                updateRanges = 0;
                            }
                            
                            //}
                        }
                        
                        var removeSamples = traceModel.SetReadSamples(samplesToClear).Result;
                        if (removeSamples.succesfulResponse)
                        {
                            samplesToClear = 0;
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

        public BulkObservableCollection<DataPoint> VSenseCollection { get; set; }

        public BulkObservableCollection<DataPoint> ISenseCollection { get; set; }

        public BulkObservableCollection<DataPoint> IRefCollection { get; set; }

        public BulkObservableCollection<DataPoint> TemperatureOneCollection { get; set; }

        public BulkObservableCollection<DataPoint> TemperatureTwoCollection { get; set; }

        public BulkObservableCollection<DataPoint> TemperatureThreeCollection { get; set; }

        public BulkObservableCollection<DataPoint> TemperatureFourCollection { get; set; }

    }
}
