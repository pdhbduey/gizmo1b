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

        Task<CommunicationData> ReadLedTemperatureCommand();

        Task<CommunicationData> ReadPdTemperatureCommand();

        Task<CommunicationData> ReadLedTemperatureDuringIntegrationCommand();

        Task<CommunicationData> ReadPdTemperatureDuringIntegrationCommand();

        Task<CommunicationData> SetPdBoardVersion(int version);

        Task<CommunicationData> SetLedBoardVersion(int version);

        Task<CommunicationData> ReadLedMonitorVolts();

        Task<CommunicationData> LedBoardEnableCommand();

        Task<CommunicationData> LedBoardDisableCommand();

        Task<CommunicationData> PdBoardEnableCommand();

        Task<CommunicationData> PdBoardDisableCommand();

        Task<CommunicationData> ResetControlSettingsCommand();

        Task<CommunicationData> ReadPhotodiodeControl();

        Task<CommunicationData> ReadIntegrationTime();

        Task<CommunicationData> ReadIntensity();

        Task<CommunicationData> ReadLedBoardVersion();

        Task<CommunicationData> ReadPdBoardVersion();
    }
}
