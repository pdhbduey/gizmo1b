using Common;
using Common.Bindings;
using DeviceManager.DeviceCommunication;
using DeviceManager.Model;
using System;
using System.Collections.Generic;
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
            SetNumberOfSamplesCommand = new RelayCommand(param => SetNumberOfSamples());
            SaveCommand = new RelayCommand(param => SaveSamples());
            BrowseCommand = new RelayCommand(param => Browse());
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

        private void Browse()
        {

        }

        public RelayCommand BrowseCommand { get; set; }

        private void SaveSamples()
        {
            var thread = new Thread(() =>
            {
                SaveSamplesThread();
            });

            thread.SetApartmentState(ApartmentState.STA);
            thread.Start();
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

                    for (var sampleNumber = 0; sampleNumber < numberOfSamples; sampleNumber++)
                    {
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

                        vSenseData.Add(Helper.GetFloatFromBigEndian(vSense.response).ToString("0.##"));
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

                        iRefData.Add(Helper.GetFloatFromBigEndian(iRef.response).ToString("0.##"));
                        errorCounter = 0;

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
                        iSenseData.Add(Helper.GetFloatFromBigEndian(iSense.response).ToString("0.##"));

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

                        temperatureOneData.Add(Helper.GetFloatFromBigEndian(temperatureOne.response).ToString("0.##"));
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

                        temperatureTwoData.Add(Helper.GetFloatFromBigEndian(temperatureTwo.response).ToString("0.##"));
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

                        temperatureThreeData.Add(Helper.GetFloatFromBigEndian(temperatureThree.response).ToString("0.##"));
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

                        temperatureFourData.Add(Helper.GetFloatFromBigEndian(temperatureFour.response).ToString("0.##"));
                        errorCounter = 0;

                        await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            ProgressText = $"{sampleNumber} / {NumberOfSamples}";
                            SaveValue = sampleNumber;
                        }));
                    }

                    // Save to csv
                    if (File.Exists(fileName))
                    {
                        File.Delete(fileName);
                    }
                    
                    using (FileStream file = new FileStream(fileName, FileMode.Create))
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
