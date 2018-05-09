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


        private int updateDelay = 200;

        public AdcViewModel(IAdcModel adcModel)
        {
            this.adcModel = adcModel;
            InitialUpdate(10);

            RefreshCommand = new RelayCommand(param => InitialUpdate(10));
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

        //private void StartUpdateTask()
        //{
        //    updateTask = Task.Factory.StartNew(() =>
        //    {
        //        UpdateAllStatuses();
        //    });
        //}

        private void InitialUpdate(int delay)
        {
            // Channel Zero
            var response = new byte[5];
            adcModel.ControlAdcChannel(0, ref response);
            response = new byte[5];

            if (adcModel.ReadStatus(ref response))
            {
                var statusChannelZero = response;
                if (ProcessStatus(statusChannelZero, 0))
                {
                    response = new byte[5];
                    if (adcModel.ReadAdcResult(ref response))
                    {
                        var channelZeroResult = response;
                        ChannelZeroValue = Helper.GetFloatFromBigEndian(channelZeroResult);
                    }
                }
            }

            Thread.Sleep(delay);

            //// Channel One
            response = new byte[5];
            adcModel.ControlAdcChannel(1, ref response);
            response = new byte[5];

            if (adcModel.ReadStatus(ref response))
            {
                var statusChannelOne = response;
                if (ProcessStatus(statusChannelOne, 1))
                {
                    response = new byte[5];
                    if (adcModel.ReadAdcResult(ref response))
                    {
                        var channelOneResult = response;
                        ChannelOneValue = Helper.GetFloatFromBigEndian(channelOneResult);
                    }
                }
            }

            Thread.Sleep(delay);

            // Channel Two
            response = new byte[5];
            adcModel.ControlAdcChannel(2, ref response);
            response = new byte[5];

            if (adcModel.ReadStatus(ref response))
            {
                var statusChannelTwo = response;
                if (ProcessStatus(statusChannelTwo, 2))
                {
                    response = new byte[5];
                    if (adcModel.ReadAdcResult(ref response))
                    {
                        var channelTwoResult = response;
                        ChannelTwoValue = Helper.GetFloatFromBigEndian(channelTwoResult);
                    }
                }
            }

            Thread.Sleep(delay);

            // Channel Three
            response = new byte[5];
            adcModel.ControlAdcChannel(3, ref response);
            response = new byte[5];

            if (adcModel.ReadStatus(ref response))
            {
                var statusChannelThree = response;
                if (ProcessStatus(statusChannelThree, 3))
                {
                    response = new byte[5];
                    if (adcModel.ReadAdcResult(ref response))
                    {
                        var channelThreeResult = response;
                        ChannelThreeValue = Helper.GetFloatFromBigEndian(channelThreeResult);
                    }
                }
            }

            Thread.Sleep(delay);

            // Channel Four
            response = new byte[5];
            adcModel.ControlAdcChannel(4, ref response);
            response = new byte[5];

            if (adcModel.ReadStatus(ref response))
            {
                var statusChannelFour = response;
                if (ProcessStatus(statusChannelFour, 4))
                {
                    response = new byte[5];
                    if (adcModel.ReadAdcResult(ref response))
                    {
                        var channelFourResult = response;
                        ChannelFourValue = Helper.GetFloatFromBigEndian(channelFourResult);
                    }
                }
            }

            Thread.Sleep(delay);

            // Channel Five
            response = new byte[5];
            adcModel.ControlAdcChannel(5, ref response);
            response = new byte[5];

            if (adcModel.ReadStatus(ref response))
            {
                var statusChannelFive = response;
                if (ProcessStatus(statusChannelFive, 5))
                {
                    response = new byte[5];
                    if (adcModel.ReadAdcResult(ref response))
                    {
                        var channelFiveResult = response;
                        ChannelFiveValue = Helper.GetFloatFromBigEndian(channelFiveResult);
                    }
                }
            }

            Thread.Sleep(delay);
        }

        private void updateEvent(object sender, EventArgs e)
        {
            Application.Current.Dispatcher.BeginInvoke((Action)delegate
            {
                InitialUpdate(10);


            });
        }

        private void UpdateAllStatuses()
        {
            var completedPreviousUpdate = true;

            while (true)
            {
                if (!completedPreviousUpdate)
                {
                    Thread.Sleep(updateDelay);
                }
                else
                {
                    completedPreviousUpdate = false;
                    try
                    {



                        ////new Func<int>(() => { System.Threading.Thread.Sleep(5000); return 1; })
                        Application.Current.Dispatcher.InvokeAsync((Action)delegate
                        {
                            InitialUpdate(10);
                            Thread.Sleep(updateDelay);
                            completedPreviousUpdate = true;
                        });
                        //updatedEvents.Invoke(null, null);
                        //InitialUpdate(delayBetweenRequests);
                    }
                    catch (Exception e)
                    {
                        StatusMessage = e.Message;
                        completedPreviousUpdate = true;
                    }
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
