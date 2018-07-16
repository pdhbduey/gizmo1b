using Common;
using Common.Bindings;
using DeviceManager.DeviceCommunication;
using DeviceManager.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace DeviceManager.ViewModel
{
    public class SnapshotViewModel : BindableBase
    {
        private ISnapshotModel snapshotModel;
        private int selectedResolution;
        private int numberOfSamples;
        private double sampleInterval;
        private int saveValue;
        private string progressText;
        private string fileName;
        private bool isNotSaving;
        private bool isCancelEnabled;
        private bool isStopEnabled;
        private bool isStartEnabled;
        private bool isSaveEnabled;
        private const int numberOfErrorsBeforeRetry = 50;

        private CancellationTokenSource cts;
        private CancellationToken token;
        private Task saveTask;

        public SnapshotViewModel(ISnapshotModel snapshotModel)
        {
            this.snapshotModel = snapshotModel;

            // InitialUpdate();
            progressText = string.Empty;
            fileName = string.Empty;
            isNotSaving = true;
            RawDataSelected = false;

            Resolutions = SnapshotDefaults.Resolutions;
            SelectedResolution = Resolutions[0];
            NumberOfSamples = SnapshotDefaults.SampleNumberMinimum;
            SampleMinimumX = 0;
            SampleMaximumX = (double)numberOfSamples / (double)selectedResolution;

            StartCommand = new RelayCommand(param => Start());
            StopCommand = new RelayCommand(param => Stop());
            CancelCommand = new RelayCommand(param => Cancel());
            SetNumberOfSamplesCommand = new RelayCommand(param => SetNumberOfSamples());
            SaveCommand = new RelayCommand(param => SaveSamples());
            BrowseCommand = new RelayCommand(param => Browse());

            VSenseCollection = new BulkObservableCollection<DataPoint>();
            IRefCollection = new BulkObservableCollection<DataPoint>();
            ISenseCollection = new BulkObservableCollection<DataPoint>();
            TemperatureOneCollection = new BulkObservableCollection<DataPoint>(); 
            TemperatureTwoCollection = new BulkObservableCollection<DataPoint>();
            TemperatureThreeCollection = new BulkObservableCollection<DataPoint>();
            TemperatureFourCollection = new BulkObservableCollection<DataPoint>();
        }

        private enum ButtonAction
        {
            State1,
            State2,
            State3,
            State4,
            State5,
            State6
        }

        private void SetEnableStatuses(ButtonAction action)
        {
            switch (action)
            {
                case ButtonAction.State1:
                    IsStartEnabled = false;
                    IsStopEnabled = true;
                    IsCancelEnabled = false;
                    IsSaveEnabled = false;
                    break;
                case ButtonAction.State2:
                    IsStartEnabled = false;
                    IsStopEnabled = true;
                    IsCancelEnabled = false;
                    IsSaveEnabled = false;
                    break;
                case ButtonAction.State3:
                    IsStartEnabled = false;
                    IsStopEnabled = true;
                    IsCancelEnabled = false;
                    IsSaveEnabled = false;
                    break;
                case ButtonAction.State4:
                    IsStartEnabled = false;
                    IsStopEnabled = true;
                    IsCancelEnabled = false;
                    IsSaveEnabled = false;
                    break;
                case ButtonAction.State5:
                    IsStartEnabled = false;
                    IsStopEnabled = true;
                    IsCancelEnabled = false;
                    IsSaveEnabled = false;
                    break;
                case ButtonAction.State6:
                    IsStartEnabled = false;
                    IsStopEnabled = true;
                    IsCancelEnabled = false;
                    IsSaveEnabled = false;
                    break;
            }
        }

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

        public bool RawDataSelected { get; set; }

        private double sampleMinimumX;
        private double sampleMaximumX;

        public double SampleInterval
        {
            get
            {
                return sampleInterval;
            }

            set
            {
                sampleInterval = value;
                OnPropertyChanged(nameof(SampleInterval));
            }
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

        public BulkObservableCollection<DataPoint> VSenseCollection { get; set; }

        public BulkObservableCollection<DataPoint> ISenseCollection { get; set; }

        public BulkObservableCollection<DataPoint> IRefCollection { get; set; }

        public BulkObservableCollection<DataPoint> TemperatureOneCollection { get; set; }

        public BulkObservableCollection<DataPoint> TemperatureTwoCollection { get; set; }

        public BulkObservableCollection<DataPoint> TemperatureThreeCollection { get; set; }

        public BulkObservableCollection<DataPoint> TemperatureFourCollection { get; set; }


        public bool IsCancelEnabled
        {
            get
            {
                return isCancelEnabled;
            }

            set
            {
                isCancelEnabled = value;
                OnPropertyChanged(nameof(IsCancelEnabled));
            }
        }

        public bool IsSaveEnabled
        {
            get
            {
                return isSaveEnabled;
            }

            set
            {
                isSaveEnabled = value;
                OnPropertyChanged(nameof(IsSaveEnabled));
            }
        }

        public bool IsStartEnabled
        {
            get
            {
                return isStartEnabled;
            }

            set
            {
                isStartEnabled = value;
                OnPropertyChanged(nameof(IsStartEnabled));
            }
        }

        public bool IsStopEnabled
        {
            get
            {
                return isStopEnabled;
            }

            set
            {
                isStopEnabled = value;
                OnPropertyChanged(nameof(IsStopEnabled));
            }
        }

        public string FileName
        {
            get
            {
                return fileName;
            }

            set
            {
                fileName = value;
                OnPropertyChanged(nameof(FileName));
            }
        }

        public RelayCommand CancelCommand { get; set; }

        public string FilePath
        {
            get
            {
                return filePath;
            }
            set
            {
                filePath = value;
                OnPropertyChanged(nameof(FilePath));
            }
        }

        private string filePath;

        private void Browse()
        {
            var browser = new System.Windows.Forms.SaveFileDialog();
            string filePath = string.Empty;
            browser.FileName = $"{browser.FileName}\\{FileName}";
            if (browser.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                filePath = browser.FileName;
            }
        }

        private void Cancel()
        {
            cts.Cancel();
        }

        public RelayCommand BrowseCommand { get; set; }

        private void SaveSamples()
        {
            cts = new CancellationTokenSource();
            token = cts.Token;

            var task = Task.Factory.StartNew(() =>
            {
                SaveSamplesThread();
            }, token);
        }

        private async void SaveSamplesThread()
        {
            var status = snapshotModel.ReadStatus().Result;
            if (status.succesfulResponse)
            {
                if (ReadyToSave(status.response))
                {
                    IsNotSaving = false;
                    ProgressText = string.Empty;

                    // Process VSense
                    List<string> vSenseData = new List<string>(numberOfSamples);
                    List<string> iSenseData = new List<string>(numberOfSamples);
                    List<string> iRefData = new List<string>(numberOfSamples);
                    List<string> temperatureOneData = new List<string>(numberOfSamples);
                    List<string> temperatureTwoData = new List<string>(numberOfSamples);
                    List<string> temperatureThreeData = new List<string>(numberOfSamples);
                    List<string> temperatureFourData = new List<string>(numberOfSamples);
                    await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        VSenseCollection.Clear();
                        ISenseCollection.Clear();
                        IRefCollection.Clear();
                        TemperatureFourCollection.Clear();
                        TemperatureThreeCollection.Clear();
                        TemperatureTwoCollection.Clear();
                        TemperatureOneCollection.Clear();
                    }));


                    for (var sampleNumber = 0; sampleNumber < numberOfSamples; sampleNumber++)
                    {
                        if (token.IsCancellationRequested == true)
                        {
                            await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                            {
                                ProgressText = "Cancelled";
                                SaveValue = 0;
                                VSenseCollection.Clear();
                                ISenseCollection.Clear();
                                IRefCollection.Clear();
                                TemperatureFourCollection.Clear();
                                TemperatureThreeCollection.Clear();
                                TemperatureTwoCollection.Clear();
                                TemperatureOneCollection.Clear();
                                IsNotSaving = true;
                            }));

                            return;
                        }

                        var sampledTime = (double)sampleNumber / (double)SelectedResolution;
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            SampleMaximumX = (double)numberOfSamples / (double)selectedResolution;
                        }));

                        
                        var errorCounter = 0;

                        CommunicationData vSense;
                        do
                        {
                            vSense = snapshotModel.ReadVsenseSamples(sampleNumber).Result;
                            errorCounter++;
                            if (!vSense.succesfulResponse)
                            {
                                Thread.Sleep(100);
                            }

                            if (errorCounter > numberOfErrorsBeforeRetry)
                            {
                                ProgressText = "Too many unsuccesful communication attempts, please try again.";
                                IsNotSaving = true;
                                return;
                            }
                        } while (!vSense.succesfulResponse);

                        vSenseData.Add(ReadDataInSelectedFormat(vSense.response, out var vSenseDataValue));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            VSenseCollection.SurpressedAdd(new DataPoint(sampledTime, vSenseDataValue), numberOfSamples);
                        }));

                        errorCounter = 0;

                        CommunicationData iRef;
                        do
                        {
                            iRef = snapshotModel.ReadIrefSamples(sampleNumber).Result;
                            errorCounter++;
                            if (!iRef.succesfulResponse)
                            {
                                Thread.Sleep(100);
                            }

                            if (errorCounter > numberOfErrorsBeforeRetry)
                            {
                                ProgressText = "Too many unsuccesful communication attempts, please try again.";
                                IsNotSaving = true;
                                return;
                            }
                        } while (!iRef.succesfulResponse);

                        var iRefDataValue = Helper.GetFloatFromBigEndian(iRef.response);
                        iRefData.Add(iRefDataValue.ToString());
                        errorCounter = 0;
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            IRefCollection.SurpressedAdd(new DataPoint(sampledTime, iRefDataValue), numberOfSamples);
                        }));

                        CommunicationData iSense;
                        do
                        {
                            iSense = snapshotModel.ReadIsenseSamples(sampleNumber).Result;
                            errorCounter++;

                            if (!iSense.succesfulResponse)
                            {
                                Thread.Sleep(100);
                            }
                            
                            if (errorCounter > numberOfErrorsBeforeRetry)
                            {
                                ProgressText = "Too many unsuccesful communication attempts, please try again.";
                                IsNotSaving = true;
                                return;
                            }
                        } while (!iSense.succesfulResponse);

                        errorCounter = 0;
                        iSenseData.Add(ReadDataInSelectedFormat(iSense.response, out var iSenseDataValue));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            ISenseCollection.SurpressedAdd(new DataPoint(sampledTime, iSenseDataValue), numberOfSamples);
                        }));

                        CommunicationData temperatureOne;
                        do
                        {
                            temperatureOne = snapshotModel.ReadTempOneSamples(sampleNumber).Result;
                            errorCounter++;
                            if (!temperatureOne.succesfulResponse)
                            {
                                Thread.Sleep(100);
                            }

                            if (errorCounter > numberOfErrorsBeforeRetry)
                            {
                                ProgressText = "Too many unsuccesful communication attempts, please try again.";
                                IsNotSaving = true;
                                return;
                            }
                        } while (!temperatureOne.succesfulResponse);

                        temperatureOneData.Add(ReadDataInSelectedFormat(temperatureOne.response, out var temperatureOneDataValue));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TemperatureOneCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureOneDataValue), numberOfSamples);
                        }));

                        errorCounter = 0;

                        CommunicationData temperatureTwo;
                        do
                        {
                            temperatureTwo = snapshotModel.ReadTempTwoSamples(sampleNumber).Result;
                            errorCounter++;
                            if (!temperatureTwo.succesfulResponse)
                            {
                                Thread.Sleep(100);
                            }

                            if (errorCounter > numberOfErrorsBeforeRetry)
                            {
                                ProgressText = "Too many unsuccesful communication attempts, please try again.";
                                IsNotSaving = true;
                                return;
                            }
                        } while (!temperatureTwo.succesfulResponse);

                        temperatureTwoData.Add(ReadDataInSelectedFormat(temperatureTwo.response, out var temperatureTwoDataValue));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TemperatureTwoCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureTwoDataValue), numberOfSamples);
                        }));
                        errorCounter = 0;

                        CommunicationData temperatureThree;
                        do
                        {
                            temperatureThree = snapshotModel.ReadTempThreeSamples(sampleNumber).Result;
                            errorCounter++;
                            if (!temperatureThree.succesfulResponse)
                            {
                                Thread.Sleep(100);
                            }

                            if (errorCounter > numberOfErrorsBeforeRetry)
                            {
                                ProgressText = "Too many unsuccesful communication attempts, please try again.";
                                IsNotSaving = true;
                                return;
                            }
                        } while (!temperatureThree.succesfulResponse);

                        temperatureThreeData.Add(ReadDataInSelectedFormat(temperatureThree.response, out var temperatureThreeDataValue));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TemperatureThreeCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureThreeDataValue), numberOfSamples);
                        }));
                        errorCounter = 0;

                        CommunicationData temperatureFour;
                        do
                        {
                            temperatureFour = snapshotModel.ReadTempFourSamples(sampleNumber).Result;
                            errorCounter++;
                            if (!temperatureFour.succesfulResponse)
                            {
                                Thread.Sleep(100);
                            }

                            if (errorCounter > numberOfErrorsBeforeRetry)
                            {
                                ProgressText = "Too many unsuccesful communication attempts, please try again.";
                                IsNotSaving = true;
                                return;
                            }

                        } while (!temperatureFour.succesfulResponse);

                        temperatureFourData.Add(ReadDataInSelectedFormat(temperatureFour.response, out var temperatureFourDataValue));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TemperatureFourCollection.SurpressedAdd(new DataPoint(sampledTime, temperatureFourDataValue), numberOfSamples);
                        }));
                        errorCounter = 0;

                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            ProgressText = $"{sampleNumber} / {NumberOfSamples}";
                            SaveValue = sampleNumber;
                        }));
                    }

                    // Save to csv
                    
                    var fileFullName = $"\\{FileName} {DateTime.Now.ToString("MM dd yyyy HH mm ss")}.csv";
                    var filePath = Directory.GetCurrentDirectory() + fileFullName;

                    if (File.Exists(filePath))
                    {
                        File.Delete(filePath);
                    }
                    
                    try
                    {
                        using (FileStream file = new FileStream(filePath, FileMode.Create))
                        {
                            using (StreamWriter sw = new StreamWriter(file))
                            {
                                sw.WriteLine(string.Format("vSense, iSense, iRef, Temperature1, Temperature2, Temperature3, Temperature4"));

                                for (var sampleNumber = 0; sampleNumber < numberOfSamples; sampleNumber++)
                                {
                                    sw.WriteLine(string.Format($"{vSenseData[sampleNumber]}, {iSenseData[sampleNumber]}, {iRefData[sampleNumber]}, {temperatureOneData[sampleNumber]}, {temperatureTwoData[sampleNumber]}, {temperatureThreeData[sampleNumber]}, {temperatureFourData[sampleNumber]}"));
                                }
                            }
                        }
                    }
                    catch
                    {
                        ProgressText = "Exception Thrown, Save incomplete.";
                        SaveValue = 0;
                        VSenseCollection.Clear();
                        ISenseCollection.Clear();
                        IRefCollection.Clear();
                        TemperatureFourCollection.Clear();
                        TemperatureThreeCollection.Clear();
                        TemperatureTwoCollection.Clear();
                        TemperatureOneCollection.Clear();
                        IsNotSaving = true;

                        if (File.Exists(filePath))
                        {
                            File.Delete(filePath);
                        }
                        return;
                    }

                    ProgressText = "Complete";
                    SaveValue = 0;
                    IsNotSaving = true;
                }
            }
            else
            {
                ProgressText = "Unsuccesful communication";
            }
        }

        private string ReadDataInSelectedFormat(byte[] data, out double dataValue)
        {
            if (RawDataSelected)
            {
                dataValue = Helper.GetFloatFromBigEndian(data);
            }
            else
            {
                dataValue = Helper.GetIntFromBigEndian(data);
            }

            return dataValue.ToString();
        }

        private bool ReadyToSave(byte[] status)
        {
            ProgressText = SnapshotDefaults.StatusCodes[status[4]];

            if (status[4] == 0x00)
            {
                return true;
            }

            return false;
        }

        public string ProgressText
        {
            get
            {
                return progressText;
            }

            set
            {
                progressText = value;
                OnPropertyChanged(nameof(ProgressText));
            }
        }

        public int SaveValue
        {
            get
            {
                return saveValue;
            }

            set
            {
                saveValue = value;
                OnPropertyChanged(nameof(SaveValue));
            }
        }

        public bool IsNotSaving
        {
            get
            {
                return isNotSaving;
            }

            set
            {
                isNotSaving = value;
                OnPropertyChanged(nameof(IsNotSaving));
                OnPropertyChanged(nameof(IsSaving));
            }
        }

        public bool IsSaving
        {
            get
            {
                return !isNotSaving;
            }
        }

        public RelayCommand SaveCommand { get; set; }

        public RelayCommand StartCommand { get; set; }

        public RelayCommand StopCommand { get; set; }

        public RelayCommand SetNumberOfSamplesCommand { get; set; }

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
                SampleInterval = (double)NumberOfSamples / 100;
            }
        }

        private async void SetResolution()
        {
            await snapshotModel.SetResolution(selectedResolution);
        }

        private void Start()
        {
            snapshotModel.StartSnapshot(RawDataSelected).Wait();
        }

        private async void Stop()
        {
            cts?.Cancel();
            await snapshotModel.StopSnapshot(RawDataSelected);
        }

        private async void SetNumberOfSamples()
        {
            if (numberOfSamples > SnapshotDefaults.SampleNumberMaximum)
            {
                NumberOfSamples = SnapshotDefaults.SampleNumberMaximum;
            }
            else if (numberOfSamples < SnapshotDefaults.SampleNumberMinimum)
            {
                NumberOfSamples = SnapshotDefaults.SampleNumberMinimum;
            }

            await snapshotModel.SetNumberOfSamples(numberOfSamples);
        }
    }
}
