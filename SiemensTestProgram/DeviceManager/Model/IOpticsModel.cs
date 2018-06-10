using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IOpticsModel
    {
        Task<CommunicationData> ReadStatusCommand();

        Task<CommunicationData> SetLedCommand(string led);

        Task<CommunicationData> SetPhotodiodeCommand(string photodiode);

        Task<CommunicationData> SetIntegrationTimeCommand(int integrationTime);

        Task<CommunicationData> SetIntensityCommand(int intensity);

        Task<CommunicationData> ReadPhotodiodeVoltsCommand();

        Task<CommunicationData> ReadPhotodiodeRawCommand();
    }
}
