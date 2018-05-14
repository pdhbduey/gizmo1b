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
            
        }

        public Task<CommunicationData> StopSnapshot()
        {

        }

        public Task<CommunicationData> SetResolution()
        {

        }

        public Task<CommunicationData> SetNumberOfSamples()
        {

        }

        public Task<CommunicationData> ReadStatus()
        {

        }

        public Task<CommunicationData> ReadVsenseSamples()
        {

        }

        public Task<CommunicationData> ReadIsenseSamples()
        {

        }

        public Task<CommunicationData> ReadIrefSamples()
        {

        }

        public Task<CommunicationData> ReadTempOneSamples()
        {

        }

        public Task<CommunicationData> ReadTempTwoSamples()
        {

        }

        public Task<CommunicationData> ReadTempThreeSamples()
        {

        }

        public Task<CommunicationData> ReadTempFourSamples()
        {

        }
    }
}
