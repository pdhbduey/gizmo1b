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
    using Model;

    public class AdcViewModel : BindableBase
    {
        private IAdcModel adcModel;
        //private SynchronizationContext AdcUiThread;

        private float channelZeroValue;
        private float channelOneValue;
        private float channelTwoValue;
        private float channelThreeValue;
        private float channelFourValue;
        private float channelFiveValue;
        private string statusMessage;


        private int updateDelay = 300;

        public AdcViewModel(IAdcModel adcModel)
        {
            this.adcModel = adcModel;
            InitialUpdate(10);

            StartUpdateTask();
            //RefreshCommand = new RelayCommand(param => InitialUpdate(10));
        }

        public RelayCommand RefreshCommand { get; set; }

        public string ChannelZeroValueText
        {
            get
            {
                return $"Channel 0: {channelZeroValue.ToString("0.##")} V";
            }
        }

        public float ChannelZeroValue
        {
            get
            {
                return channelZeroValue;
            }

            set
            {
                channelZeroValue = value;
                OnPropertyChanged(nameof(ChannelZeroValueText));
            }
        }

        public string ChannelOneValueText
        {
            get
            {
                return $"Channel 1: {channelOneValue.ToString("0.##")} V";
            }
        }

        public float ChannelOneValue
        {
            get
            {
                return channelOneValue;
            }

            set
            {
                channelOneValue = value;
                OnPropertyChanged(nameof(ChannelOneValueText));
            }
        }

        public string ChannelTwoValueText
        {
            get
            {
                return $"Channel 2: {channelTwoValue.ToString("0.##")} V";
            }
        }

        public float ChannelTwoValue
        {
            get
            {
                return channelTwoValue;
            }

            set
            {
                channelTwoValue = value;
                OnPropertyChanged(nameof(ChannelTwoValueText));
            }
        }

        public string ChannelThreeValueText
        {
            get
            {
                return $"Channel 3: {channelThreeValue.ToString("0.##")} V";
            }
        }

        public float ChannelThreeValue
        {
            get
            {
                return channelThreeValue;
            }

            set
            {
                channelThreeValue = value;
                OnPropertyChanged(nameof(ChannelThreeValueText));
            }
        }

        public string ChannelFourValueText
        {
            get
            {
                return $"Channel 4: {channelFourValue.ToString("0.##")} V";
            }
        }

        public float ChannelFourValue
        {
            get
            {
                return channelFourValue;
            }

            set
            {
                channelFourValue = value;
                OnPropertyChanged(nameof(ChannelFourValueText));
            }
        }

        public string ChannelFiveValueText
        {
            get
            {
                return $"Channel 5: {channelFiveValue.ToString("0.##")} V";
            }
        }

        public float ChannelFiveValue
        {
            get
            {
                return channelFiveValue;
            }

            set
            {
                channelFiveValue = value;
                OnPropertyChanged(nameof(ChannelFiveValueText));
            }
        }

        public string StatusMessage
        {
            get
            {
                return $"Status: {statusMessage}";
            }

            set
            {
                statusMessage = value;
                OnPropertyChanged(nameof(StatusMessage));
            }
        }

        private void StartUpdateTask()
        {
            var thread = new Thread(() =>
            {
                UpdateAllStatuses();
            });

            thread.SetApartmentState(ApartmentState.STA);
            thread.Start();
        }

        private void HandleResults(int channel)
        {
            var control = adcModel.ControlAdcChannel(channel).Result;

            var statusResponse = adcModel.ReadStatus().Result;

            if (statusResponse.succesfulResponse)
            {
                var statusChannelZero = statusResponse.response;
                if (ProcessStatus(statusChannelZero, channel))
                {
                    var result = adcModel.ReadAdcResult().Result;
                    if (result.succesfulResponse)
                    {
                        var channelZeroResult = result.response;
                        ChannelZeroValue = Helper.GetFloatFromBigEndian(channelZeroResult);
                    }
                }
            }
        }

        private void InitialUpdate(int delay)
        {
            HandleResults(0);
            Thread.Sleep(delay);

            HandleResults(1);
            Thread.Sleep(delay);

            HandleResults(2);
            Thread.Sleep(delay);

            HandleResults(3);
            Thread.Sleep(delay);

            HandleResults(4);
            Thread.Sleep(delay);

            HandleResults(5);
            Thread.Sleep(delay);
        }

        private async void UpdateAllStatuses()
        {
            while (true)
            {
                
                var commandZeroSuccesful = await adcModel.ControlAdcChannel(0);
                if (commandZeroSuccesful.succesfulResponse)
                {
                    var result = await adcModel.ReadStatus();
                    if (result.succesfulResponse)
                    {
                        var statusChannelZero = result.response;
                        if (ProcessStatus(statusChannelZero, 0))
                        {
                            var readResultChannelZero = await adcModel.ReadAdcResult();
                            if (readResultChannelZero.succesfulResponse)
                            {
                                var channelZeroResult = readResultChannelZero.response;
                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    ChannelZeroValue = Helper.GetFloatFromBigEndian(channelZeroResult);
                                }));
                            }
                        }
                    }
                }

                Thread.Sleep(updateDelay);

                //// Channel One
                var commandOneSuccesful = await adcModel.ControlAdcChannel(1);
                if (commandOneSuccesful.succesfulResponse)
                {
                    var result = await adcModel.ReadStatus();
                    if (result.succesfulResponse)
                    {
                        var statusChannelOne = result.response;
                        if (ProcessStatus(statusChannelOne, 1))
                        {
                            var readResultChannelOne = await adcModel.ReadAdcResult();
                            if (readResultChannelOne.succesfulResponse)
                            {
                                var channelOneResult = readResultChannelOne.response;
                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    ChannelOneValue = Helper.GetFloatFromBigEndian(channelOneResult);
                                }));
                            }
                        }
                    }
                }

                Thread.Sleep(updateDelay);

                // Channel Two
                var commandTwoSuccesful = await adcModel.ControlAdcChannel(2);
                if (commandTwoSuccesful.succesfulResponse)
                {
                    var result = await adcModel.ReadStatus();
                    if (result.succesfulResponse)
                    {
                        var statusChannelTwo = result.response;
                        if (ProcessStatus(statusChannelTwo, 2))
                        {
                            var readResultChannelTwo = await adcModel.ReadAdcResult();
                            if (readResultChannelTwo.succesfulResponse)
                            {
                                var channelTwoResult = readResultChannelTwo.response;
                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    ChannelTwoValue = Helper.GetFloatFromBigEndian(channelTwoResult);
                                }));
                            }
                        }
                    }
                }

                Thread.Sleep(updateDelay);

                // Channel Three
                var commandThreeSuccesful = await adcModel.ControlAdcChannel(3);
                if (commandThreeSuccesful.succesfulResponse)
                {
                    var result = await adcModel.ReadStatus();
                    if (result.succesfulResponse)
                    {
                        var statusChannelThree = result.response;
                        if (ProcessStatus(statusChannelThree, 3))
                        {
                            var readResultChannelThree = await adcModel.ReadAdcResult();
                            if (readResultChannelThree.succesfulResponse)
                            {
                                var channelThreeResult = readResultChannelThree.response;

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    ChannelThreeValue = Helper.GetFloatFromBigEndian(channelThreeResult);
                                }));
                                
                            }
                        }
                    }
                }

                Thread.Sleep(updateDelay);

                // Channel Four
                var commandFourSuccesful = await adcModel.ControlAdcChannel(4);
                if (commandFourSuccesful.succesfulResponse)
                {
                    var result = await adcModel.ReadStatus();
                    if (result.succesfulResponse)
                    {
                        var statusChannelFour = result.response;
                        if (ProcessStatus(statusChannelFour, 4))
                        {
                            var readResultChannelFour = await adcModel.ReadAdcResult();
                            if (readResultChannelFour.succesfulResponse)
                            {
                                var channelFourResult = readResultChannelFour.response;

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    ChannelFourValue = Helper.GetFloatFromBigEndian(channelFourResult);
                                }));
                                
                            }
                        }
                    }
                }

                Thread.Sleep(updateDelay);

                // Channel Five
                var commandFiveSuccesful = await adcModel.ControlAdcChannel(5);
                if (commandFiveSuccesful.succesfulResponse)
                {
                    var result = await adcModel.ReadStatus();
                    if (result.succesfulResponse)
                    {
                        var statusChannelFive = result.response;
                        if (ProcessStatus(statusChannelFive, 5))
                        {
                            var readResultChannelFive = await adcModel.ReadAdcResult();
                            if (readResultChannelFive.succesfulResponse)
                            {
                                var channelFiveResult = readResultChannelFive.response;

                                await Application.Current.Dispatcher.BeginInvoke(new Action(() =>
                                {
                                    ChannelFiveValue = Helper.GetFloatFromBigEndian(channelFiveResult);
                                }));
                                
                            }
                        }
                    }
                }

                Thread.Sleep(updateDelay);
            }
        }

        private bool ProcessStatus(byte[] status, int channelNumber)
        {
            if (status.Length < 4)
            {
                StatusMessage = "Communication Error";
                return false;
            }

            StatusMessage = GetErrorMessage(status[4], channelNumber);

            if (statusMessage.Contains("Okay"))
            {
                return true;
            }

            return false;
        }

        private string GetErrorMessage(byte value, int channelNumber)
        {
            string response;
            AdcDefaults.Errors.TryGetValue(value, out response);

            return response == null ? "Unknown" : $"Channel {channelNumber} {response}";
        }
    }
}
