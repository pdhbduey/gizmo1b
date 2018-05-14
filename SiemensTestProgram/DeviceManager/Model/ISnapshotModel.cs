using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface ISnapshotModel
    {
        Task<CommunicationData> StartSnapshot();

        Task<CommunicationData> StopSnapshot();

        Task<CommunicationData> SetResolution();

        Task<CommunicationData> SetNumberOfSamples();

        Task<CommunicationData> ReadStatus();

        Task<CommunicationData> ReadVsenseSamples();

        Task<CommunicationData> ReadIsenseSamples();

        Task<CommunicationData> ReadIrefSamples();

        Task<CommunicationData> ReadTempOneSamples();

        Task<CommunicationData> ReadTempTwoSamples();

        Task<CommunicationData> ReadTempThreeSamples();

        Task<CommunicationData> ReadTempFourSamples();
    }
}
