using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface ITraceModel
    {
        Task<CommunicationData> StartTrace();

        Task<CommunicationData> StopTrace();

        Task<CommunicationData> SetResolution(int resolution);

        Task<CommunicationData> SetNumberOfSamples(int numberOfSamples);

        Task<CommunicationData> SetReadSamples(int numberOfReadSamples);

        Task<CommunicationData> GetNumberOfAvailableSamples();

        Task<CommunicationData> GetFirstAvailableSampleIndex();

        Task<CommunicationData> ReadVsenseSamples(int sampleNumber);

        Task<CommunicationData> ReadIsenseSamples(int sampleNumber);

        Task<CommunicationData> ReadIrefSamples(int sampleNumber);

        Task<CommunicationData> ReadTempOneSamples(int sampleNumber);

        Task<CommunicationData> ReadTempTwoSamples(int sampleNumber);

        Task<CommunicationData> ReadTempThreeSamples(int sampleNumber);

        Task<CommunicationData> ReadTempFourSamples(int sampleNumber);

    }
}
