using DeviceManager.DeviceCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceManager.Model
{
    public class HeaterModel : IHeaterModel
    {
        private IComCommunication communication;

        public HeaterModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<CommunicationData> SetProportionalGainCommand(float gain)
        {
            var requestArray = HeaterDefaults.SetProportionalGain(gain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetIntegralGainCommand(float gain)
        {
            var requestArray = HeaterDefaults.SetIntegralGain(gain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetDerivativeGainCommand(float gain)
        {
            var requestArray = HeaterDefaults.SetDerivativeGain(gain);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetTRefCommand(float tRef)
        {
            var requestArray = HeaterDefaults.SetTref(tRef);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetIMaxCommand(float iMax)
        {
            var requestArray = HeaterDefaults.SetImax(iMax);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetTinCommand(string selectedTin)
        {
            int intSelectedTin;
            switch (selectedTin)
            {
                case "1":
                    intSelectedTin = 1;
                    break;
                case "2":
                    intSelectedTin = 2;
                    break;
                case "3":
                    intSelectedTin = 3;
                    break;
                case "4":
                    intSelectedTin = 4;
                    break;
                default:
                    intSelectedTin = 1;
                    break;
            }

            var requestArray = HeaterDefaults.SetTinSelect(intSelectedTin);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadTinCommand()
        {
            var requestArray = HeaterDefaults.ReadTinSelect();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadStatusCommand()
        {
            var requestArray = HeaterDefaults.ReadStatus();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadVSenseCommand()
        {
            var requestArray = TecDefaults.GetVsenseCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadISenseCommand()
        {
            var requestArray = TecDefaults.GetIsenseCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadTemperatureOne()
        {
            var requestArray = ThermistorDefaults.GetReadAinACommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadTemperatureTwo()
        {
            var requestArray = ThermistorDefaults.GetReadAinBCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadTemperatureThree()
        {
            var requestArray = ThermistorDefaults.GetReadAinCCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadTemperatureFour()
        {
            var requestArray = ThermistorDefaults.GetReadAinDCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> StartClosedLoopCommand()
        {
            var requestArray = HeaterDefaults.SetEnableClosedLoopCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> StopClosedLoopCommand()
        {
            var requestArray = HeaterDefaults.SetDisableClosedLoopCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> EnableCommand()
        {
            var requestArray = HeaterDefaults.SetEnableCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> DisableCommand()
        {
            var requestArray = HeaterDefaults.SetDisableCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
