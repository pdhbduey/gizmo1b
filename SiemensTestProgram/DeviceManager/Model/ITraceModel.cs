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

    }
}
