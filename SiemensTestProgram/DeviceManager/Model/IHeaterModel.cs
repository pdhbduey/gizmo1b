using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public interface IHeaterModel
    {
        Task<CommunicationData> SetProportionalGainCommand(float gain);

        Task<CommunicationData> SetIntegralGainCommand(float gain);

        Task<CommunicationData> SetDerivativeGainCommand(float gain);

        Task<CommunicationData> SetTRefCommand(float tRef);

        Task<CommunicationData> SetIMaxCommand(float iMax);

        Task<CommunicationData> SetTinCommand(string selectedTin);

        Task<CommunicationData> ReadTinCommand();

        Task<CommunicationData> ReadStatusCommand();

        Task<CommunicationData> StartClosedLoopCommand();

        Task<CommunicationData> StopClosedLoopCommand();

        Task<CommunicationData> EnableCommand();

        Task<CommunicationData> DisableCommand();

        Task<CommunicationData> ReadVSenseCommand();

        Task<CommunicationData> ReadISenseCommand();

        Task<CommunicationData> ReadTemperatureOne();

        Task<CommunicationData> ReadTemperatureTwo();

        Task<CommunicationData> ReadTemperatureThree();

        Task<CommunicationData> ReadTemperatureFour();
    }
}
