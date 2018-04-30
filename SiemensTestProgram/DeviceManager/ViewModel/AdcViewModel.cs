// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.ViewModel
{
    using System;
    using System.Collections.Generic;
    using System.Threading;
    using System.Threading.Tasks;
    using Common;
    using Common.Bindings;
    using Model;

    public class AdcViewModel : BindableBase
    {
        private IAdcModel adcModel;

        private float channelZeroValue;
        private float channelOneValue;
        private float channelTwoValue;
        private float channelThreeValue;
        private float channelFourValue;
        private float channelFiveValue;
        private string statusMessage;

        private Task updateTask;
        CancellationTokenSource cts;
        CancellationToken token;
        private int updateDelay = 500;

        public AdcViewModel(IAdcModel adcModel)
        {
            this.adcModel = adcModel;
            InitialUpdate();
            StartUpdateTask();
        }

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
            cts = new CancellationTokenSource();
            token = cts.Token;

            updateTask = Task.Factory.StartNew(() =>
            {
                UpdateAllStatuses();
            }, token);
        }

        private void InitialUpdate()
        {
            adcModel.ControlAdcChannel(0).Wait();
            var statusChannelZero = adcModel.ReadStatus().Result;
            if (ProcessStatus(statusChannelZero, 0))
            {
                var channelZeroResult = adcModel.ReadAdcResult().Result;
                ChannelZeroValue = Helper.GetFloatFromBigEndian(channelZeroResult);
            }

            Thread.Sleep(10);

            //// Channel One
            adcModel.ControlAdcChannel(1).Wait();
            var statusChannelOne = adcModel.ReadStatus().Result;
            if (ProcessStatus(statusChannelOne, 1))
            {
                var channelOneResult = adcModel.ReadAdcResult().Result;
                ChannelOneValue = Helper.GetFloatFromBigEndian(channelOneResult);
            }

            Thread.Sleep(10);

            // Channel Two
            adcModel.ControlAdcChannel(2).Wait();
            var statusChannelTwo = adcModel.ReadStatus().Result;
            if (ProcessStatus(statusChannelTwo, 2))
            {
                var channelTwoResult = adcModel.ReadAdcResult().Result;
                ChannelTwoValue = Helper.GetFloatFromBigEndian(channelTwoResult);
            }

            Thread.Sleep(10);

            // Channel Three
            adcModel.ControlAdcChannel(3).Wait();
            var statusChannelThree = adcModel.ReadStatus().Result;
            if (ProcessStatus(statusChannelThree, 3))
            {
                var channelThreeResult = adcModel.ReadAdcResult().Result;
                ChannelThreeValue = Helper.GetFloatFromBigEndian(channelThreeResult);
            }

            Thread.Sleep(10);

            // Channel Four
            adcModel.ControlAdcChannel(4).Wait();
            var statusChannelFour = adcModel.ReadStatus().Result;
            if (ProcessStatus(statusChannelFour, 4))
            {
                var channelFourResult = adcModel.ReadAdcResult().Result;
                ChannelFourValue = Helper.GetFloatFromBigEndian(channelFourResult);
            }

            Thread.Sleep(10);

            // Channel Five
            adcModel.ControlAdcChannel(5).Wait();
            var statusChannelFive = adcModel.ReadStatus().Result;
            if (ProcessStatus(statusChannelFive, 5))
            {
                var channelFiveResult = adcModel.ReadAdcResult().Result;
                ChannelFiveValue = Helper.GetFloatFromBigEndian(channelFiveResult);
            }

            Thread.Sleep(10);
        }

        private async void UpdateAllStatuses()
        {
            while (true)
            {
                if (token.IsCancellationRequested == true)
                {
                    break;
                }

                try
                {
                    // Channel Zero
                    await adcModel.ControlAdcChannel(0);
                    var statusChannelZero = await adcModel.ReadStatus();
                    if (ProcessStatus(statusChannelZero, 0))
                    {
                        var channelZeroResult = await adcModel.ReadAdcResult();
                        ChannelZeroValue = Helper.GetFloatFromBigEndian(channelZeroResult);
                    }

                    Thread.Sleep(updateDelay);

                    //// Channel One
                    await adcModel.ControlAdcChannel(1);
                    var statusChannelOne = await adcModel.ReadStatus();
                    if (ProcessStatus(statusChannelOne, 1))
                    {
                        var channelOneResult = await adcModel.ReadAdcResult();
                        ChannelOneValue = Helper.GetFloatFromBigEndian(channelOneResult);
                    }

                    Thread.Sleep(updateDelay);

                    // Channel Two
                    await adcModel.ControlAdcChannel(2);
                    var statusChannelTwo = await adcModel.ReadStatus();
                    if (ProcessStatus(statusChannelTwo, 2))
                    {
                        var channelTwoResult = await adcModel.ReadAdcResult();
                        ChannelTwoValue = Helper.GetFloatFromBigEndian(channelTwoResult);
                    }

                    Thread.Sleep(updateDelay);

                    // Channel Three
                    await adcModel.ControlAdcChannel(3);
                    var statusChannelThree = await adcModel.ReadStatus();
                    if (ProcessStatus(statusChannelThree, 3))
                    {
                        var channelThreeResult = await adcModel.ReadAdcResult();
                        ChannelThreeValue = Helper.GetFloatFromBigEndian(channelThreeResult);
                    }

                    Thread.Sleep(updateDelay);

                    // Channel Four
                    await adcModel.ControlAdcChannel(4);
                    var statusChannelFour = await adcModel.ReadStatus();
                    if (ProcessStatus(statusChannelFour, 4))
                    {
                        var channelFourResult = await adcModel.ReadAdcResult();
                        ChannelFourValue = Helper.GetFloatFromBigEndian(channelFourResult);
                    }

                    Thread.Sleep(updateDelay);

                    // Channel Five
                    await adcModel.ControlAdcChannel(5);
                    var statusChannelFive = await adcModel.ReadStatus();
                    if (ProcessStatus(statusChannelFive, 5))
                    {
                        var channelFiveResult = await adcModel.ReadAdcResult();
                        ChannelFiveValue = Helper.GetFloatFromBigEndian(channelFiveResult);
                    }

                    Thread.Sleep(updateDelay);
                }
                catch (Exception e)
                {
                    StatusMessage = e.Message;
                }
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
