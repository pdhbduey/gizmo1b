﻿using Common;
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
        private int saveValue;
        private string progressText;
        private string fileName;
        private bool isNotSaving;
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

            Resolutions = SnapshotDefaults.Resolutions;
            SelectedResolution = Resolutions[0];

            NumberOfSamples = SnapshotDefaults.SampleNumberMinimum;

            StartCommand = new RelayCommand(param => Start());
            StopCommand = new RelayCommand(param => Stop());
            CancelCommand = new RelayCommand(param => Cancel());
            SetNumberOfSamplesCommand = new RelayCommand(param => SetNumberOfSamples());
            SaveCommand = new RelayCommand(param => SaveSamples());
            BrowseCommand = new RelayCommand(param => Browse());

            VSenseCollection = new ObservableCollection<DataPoint>();
            IRefCollection = new ObservableCollection<DataPoint>();
            ISenseCollection = new ObservableCollection<DataPoint>();
            TemperatureOneCollection = new ObservableCollection<DataPoint>(); 
            TemperatureTwoCollection = new ObservableCollection<DataPoint>();
            TemperatureThreeCollection = new ObservableCollection<DataPoint>();
            TemperatureFourCollection = new ObservableCollection<DataPoint>();
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

        public ObservableCollection<DataPoint> VSenseCollection { get; set; }

        public ObservableCollection<DataPoint> ISenseCollection { get; set; }

        public ObservableCollection<DataPoint> IRefCollection { get; set; }

        public ObservableCollection<DataPoint> TemperatureOneCollection { get; set; }

        public ObservableCollection<DataPoint> TemperatureTwoCollection { get; set; }

        public ObservableCollection<DataPoint> TemperatureThreeCollection { get; set; }

        public ObservableCollection<DataPoint> TemperatureFourCollection { get; set; }


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

                        var vSenseDataValue = Helper.GetFloatFromBigEndian(vSense.response);
                        vSenseData.Add(vSenseDataValue.ToString("0.##"));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            VSenseCollection.Add(new DataPoint(sampledTime, vSenseDataValue));
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
                        iRefData.Add(iRefDataValue.ToString("0.##"));
                        errorCounter = 0;
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            IRefCollection.Add(new DataPoint(sampledTime, iRefDataValue));
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
                        var iSenseDataValue = Helper.GetFloatFromBigEndian(iSense.response);
                        iSenseData.Add(Helper.GetFloatFromBigEndian(iSense.response).ToString("0.##"));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            ISenseCollection.Add(new DataPoint(sampledTime, iSenseDataValue));
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

                        var temperatureOneDataValue = Helper.GetFloatFromBigEndian(temperatureOne.response);
                        temperatureOneData.Add(Helper.GetFloatFromBigEndian(temperatureOne.response).ToString("0.##"));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TemperatureOneCollection.Add(new DataPoint(sampledTime, temperatureOneDataValue));
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

                        var temperatureTwoDataValue = Helper.GetFloatFromBigEndian(temperatureTwo.response);
                        temperatureTwoData.Add(Helper.GetFloatFromBigEndian(temperatureTwo.response).ToString("0.##"));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TemperatureTwoCollection.Add(new DataPoint(sampledTime, temperatureTwoDataValue));
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

                        var temperatureThreeDataValue = Helper.GetFloatFromBigEndian(temperatureThree.response);
                        temperatureThreeData.Add(Helper.GetFloatFromBigEndian(temperatureThree.response).ToString("0.##"));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TemperatureThreeCollection.Add(new DataPoint(sampledTime, temperatureThreeDataValue));
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

                        var temperatureFourDataDataValue = Helper.GetFloatFromBigEndian(temperatureFour.response);
                        temperatureFourData.Add(Helper.GetFloatFromBigEndian(temperatureFour.response).ToString("0.##"));
                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            TemperatureFourCollection.Add(new DataPoint(sampledTime, temperatureFourDataDataValue));
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
            }
        }

        private async void SetResolution()
        {
            await snapshotModel.SetResolution(selectedResolution);
        }

        private async void Start()
        {
            snapshotModel.StartSnapshot().Wait();
        }

        private async void Stop()
        {
            await snapshotModel.StopSnapshot();
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
