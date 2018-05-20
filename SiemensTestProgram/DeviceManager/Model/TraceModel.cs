using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public class TraceModel : ITraceModel
    {
        private IComCommunication communication;

        public TraceModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<CommunicationData> StartTrace()
        {
            var requestArray = TraceDefaults.SetStartCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> StopTrace()
        {
            var requestArray = TraceDefaults.SetStopCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetResolution(int resolution)
        {
            var requestArray = TraceDefaults.SetResolution(resolution);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetReadSamples(int numberOfReadSamples)
        {
            var requestArray = TraceDefaults.SetReadNumberOfSamples(numberOfReadSamples);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> GetNumberOfAvailableSamples()
        {
            var requestArray = TraceDefaults.GetNumberOfAvailableSamples();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> GetFirstAvailableSampleIndex()
        {
            var requestArray = TraceDefaults.GetFirstAvailableSamples();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetNumberOfSamples(int sampleNumber)
        {
            var requestArray = TraceDefaults.SetNumberOfSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadVsenseSamples(int sampleNumber)
        {
            var requestArray = SnapshotDefaults.ReadVsenseSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadIsenseSamples(int sampleNumber)
        {
            var requestArray = SnapshotDefaults.ReadIsenseSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadIrefSamples(int sampleNumber)
        {
            var requestArray = SnapshotDefaults.ReadIrefSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadTempOneSamples(int sampleNumber)
        {
            var requestArray = SnapshotDefaults.ReadTemperatureOneSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadTempTwoSamples(int sampleNumber)
        {
            var requestArray = SnapshotDefaults.ReadTemperatureTwoSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadTempThreeSamples(int sampleNumber)
        {
            var requestArray = SnapshotDefaults.ReadTemperatureThreeSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadTempFourSamples(int sampleNumber)
        {
            var requestArray = SnapshotDefaults.ReadTemperatureFourSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
