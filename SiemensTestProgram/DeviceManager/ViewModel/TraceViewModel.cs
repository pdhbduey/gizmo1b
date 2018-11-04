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

    public class TraceViewModel : BindableBase, IDisposable
    {
        private ITraceModel traceModel;
        private int selectedResolution;
        private int numberOfSamples;
        private const int resolutionRangeMultiplier = 2;

        private int refresh;
        private double refreshTime;

        private double sampleMinimumY;
        private double sampleMaximumY;
        private double sampleMinimumX;
        private double sampleMaximumX;
        private double minUpper = 25;
        private double minLower = -15;
        
        private CancellationTokenSource cts;
        private CancellationToken token;
        private Task saveTask;

        public TraceViewModel(ITraceModel traceModel)
        {
            this.traceModel = traceModel;
            Resolutions = TraceDefaults.Resolutions;
            SelectedResolution = Resolutions[0];

            VSenseCollection = new BulkObservableCollection<DataPoint>();
            IRefCollection = new BulkObservableCollection<DataPoint>();
            ISenseCollection = new BulkObservableCollection<DataPoint>();
            TemperatureOneCollection = new BulkObservableCollection<DataPoint>();
            TemperatureTwoCollection = new BulkObservableCollection<DataPoint>();
            TemperatureThreeCollection = new BulkObservableCollection<DataPoint>();
            TemperatureFourCollection = new BulkObservableCollection<DataPoint>();

            NumberOfSamples = TraceDefaults.SampleNumberMinimum;
            SampleMaximumX = 3;
            SampleMinimumX = 0;
            SampleMinimumY = minLower;
            SampleMaximumY = minUpper;

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

        public double SampleMinimumY
        {
            get
            {
                return sampleMinimumY;
            }

            set
            {
                if (value > minLower)
                {
                    sampleMinimumY = minUpper;
                }
                else
                {
                    sampleMinimumY = value;
                }

                OnPropertyChanged(nameof(SampleMinimumY));
            }
        }

        public double SampleMaximumY
        {
            get
            {
                return sampleMaximumY;
            }

            set
            {
                if (value < minUpper)
                {
                    sampleMaximumY = minUpper;
                }
                else
                {
                    sampleMaximumY = value;
                }
                
                OnPropertyChanged(nameof(SampleMaximumY));
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
            Thread.CurrentThread.Priority = ThreadPriority.Highest;
            var modValue = 1000;
            var samplesToClear = 0;
            double sampledTime = 0;
            double sampleNumber = 0;
            var updateRanges = 0;
            var m = 10;

            while (true)
            {
                //var watch = System.Diagnostics.Stopwatch.StartNew();

                if (token.IsCancellationRequested == true)
                {
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {

                        SampleMinimumX = 0;
                        SampleMaximumX = 1;
                        VSenseCollection.Clear();
                        ISenseCollection.Clear();
                        IRefCollection.Clear();
                        TemperatureFourCollection.Clear();
                        TemperatureThreeCollection.Clear();
                        TemperatureTwoCollection.Clear();
                        TemperatureOneCollection.Clear();
                    }));
                    break;
                }

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
                            //Console.WriteLine($"available samples: {availableSamples}");

                            var updateNumber = Math.Max(refresh, availableSamples);
                            for (var j = 0; j < availableSamples; j++)
                            {
                                sampledTime = sampleNumber / (double)selectedResolution;

                            if (updateRanges >= refresh)
                            {
                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    SampleMaximumX = sampledTime + refreshTime;
                                    SampleMinimumX = sampledTime ;
                                }));

                                updateRanges = 0;
                            }
                            // Update chart
                            var vSense = traceModel.ReadVsenseSamples(i).Result;
                            if (vSense.succesfulResponse)
                            {
                                var vSenseDataValue = Helper.GetFloatFromBigEndian(vSense.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    VSenseCollection.SurpressedAdd(new DataPoint(sampledTime, vSenseDataValue), refresh);

                                    if (sampleNumber > refresh)
                                    {
                                        VSenseCollection.SurpressedRemoveAt(0);
                                    }
                                }));
                            }

                            var iSense = traceModel.ReadIsenseSamples(i).Result;
                            if (iSense.succesfulResponse)
                            {
                                var iSenseDataValue = Helper.GetFloatFromBigEndian(iSense.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    ISenseCollection.SurpressedAdd(new DataPoint(sampledTime, iSenseDataValue), refresh);

                                    if (sampleNumber > refresh)
                                    {
                                        ISenseCollection.SurpressedRemoveAt(0);
                                    }
                                }));
                            }

                            var iRef = traceModel.ReadIrefSamples(i).Result;
                            if (iRef.succesfulResponse)
                            {
                                var iRefDataValue = Helper.GetFloatFromBigEndian(iRef.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    IRefCollection.SurpressedAdd(new DataPoint(sampledTime, iRefDataValue), refresh);

                                    if (sampleNumber > refresh)
                                    {
                                        IRefCollection.SurpressedRemoveAt(0);
                                    }
                                }));
                            }

                            var temperatureOne = traceModel.ReadTempOneSamples(i).Result;
                            if (temperatureOne.succesfulResponse)
                            {
                                var temperatureOneDataValue = Helper.GetFloatFromBigEndian(temperatureOne.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    TemperatureOneCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureOneDataValue), refresh);

                                    if (sampleNumber > refresh)
                                    {
                                        TemperatureOneCollection.SurpressedRemoveAt(0);
                                    }
                                }));
                            }

                            var temperatureTwo = traceModel.ReadTempTwoSamples(i).Result;
                            if (temperatureTwo.succesfulResponse)
                            {
                                var temperatureTwoDataValue = Helper.GetFloatFromBigEndian(temperatureTwo.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    TemperatureTwoCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureTwoDataValue), refresh);

                                    if (sampleNumber > refresh)
                                    {
                                        TemperatureTwoCollection.SurpressedRemoveAt(0);
                                    }
                                }));
                            }

                            var temperatureThree = traceModel.ReadTempThreeSamples(i).Result;
                            if (temperatureThree.succesfulResponse)
                            {
                                var temperatureThreeDataValue = Helper.GetFloatFromBigEndian(temperatureThree.response);

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    TemperatureThreeCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureThreeDataValue), refresh);

                                    if (sampleNumber > refresh)
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
                                        TemperatureFourCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureFourDataValue), refresh);
                                        if (sampleNumber > refresh)
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


                            
                            
                            //}
                        }
                        
                        var removeSamples = traceModel.SetReadSamples(samplesToClear).Result;
                        if (removeSamples.succesfulResponse)
                        {
                            var foo = traceModel.GetNumberOfAvailableSamples().Result;
                            
                            var foo2 = Helper.GetIntFromBigEndian(foo.response);
                            if (foo2 > 20)
                            {
                                traceModel.SetReadSamples(foo2).Wait();
                                var foo3 = traceModel.GetNumberOfAvailableSamples().Result;
                                //var foo4 = Helper.GetIntFromBigEndian(foo3.response);
                                //Console.WriteLine($"!!!available samples AFTER CLEAR: {foo4}");
                            }
                            
                            //Console.WriteLine($"Cleared: {samplesToClear} samples");
                            samplesToClear = 0;
                        }

                        //watch.Stop();
                        //var elapsedMs = watch.ElapsedMilliseconds;
                        //Console.WriteLine($"Time: {elapsedMs} ****");
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

        public void Dispose()
        {
            // do nothing
        }

    }
}
