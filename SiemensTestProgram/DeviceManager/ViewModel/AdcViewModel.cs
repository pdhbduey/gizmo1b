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
        private int updateDelay = 300;

        public AdcViewModel(IAdcModel adcModel)
        {
            this.adcModel = adcModel;

            StartUpdateTask();
        }

        public string ChannelZeroValueText
        {
            get
            {
                return $"Channel 0: {channelZeroValue} V";
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
                return $"Channel 1: {channelOneValue} V";
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
                return $"Channel 2: {channelTwoValue} V";
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
                return $"Channel 3: {channelThreeValue} V";
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
                return $"Channel 4: {channelFourValue} V";
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
                return $"Channel 5: {channelFiveValue} V";
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
                statusMessage = "Communication Error";
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

            return response == null ? "Unknown" : $"Channel {channelNumber}: {response}";
        }
    }
}
