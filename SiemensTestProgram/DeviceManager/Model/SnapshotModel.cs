using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public class SnapshotModel : ISnapshotModel
    {
        private IComCommunication communication;

        public SnapshotModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<CommunicationData> StartSnapshot()
        {
            var requestArray = SnapshotDefaults.StartSnapshot();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> StopSnapshot()
        {
            var requestArray = SnapshotDefaults.StopSnapshot();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetResolution(int resolution)
        {
            var requestArray = SnapshotDefaults.SetResolution(resolution);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetNumberOfSamples(int sampleNumber)
        {
            var requestArray = SnapshotDefaults.SetNumberOfSamples(sampleNumber);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadStatus()
        {
            var requestArray = SnapshotDefaults.ReadStatus();
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
