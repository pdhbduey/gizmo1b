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

        public Task<CommunicationData> ControlCommand(string state)
        {
            var requestArray = HeaterDefaults.SetWaveformControl(state);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetWaveformCyclesCommand(int waveformCycles)
        {
            var requestArray = HeaterDefaults.SetWaveformCycles(waveformCycles);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
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

        public Task<CommunicationData> SetCustomWaveformTRefCommand(float tRef)
        {
            var requestArray = HeaterDefaults.SetCustomWaveformTref(tRef);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetCustomWaveformTimeCommand(int time)
        {
            var requestArray = HeaterDefaults.SetCustomWaveformTime(time);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetIMaxCommand(float iMax)
        {
            var requestArray = HeaterDefaults.SetImax(iMax);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ResetCounterCommand()
        {
            var requestArray = HeaterDefaults.ResetCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> IncrementCounterCommand()
        {
            var requestArray = HeaterDefaults.IncrementCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadWaveformIndex()
        {
            var requestArray = HeaterDefaults.ReadWaveformIndexCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetTinCommand(string selectedTin)
        {
            int intSelectedTin;
            if (string.Equals(selectedTin, "T1"))
            {
                intSelectedTin = 0;
            }
            else if (string.Equals(selectedTin, "T2"))
            {
                intSelectedTin = 1;
            }
            else if (string.Equals(selectedTin, "T3"))
            {
                intSelectedTin = 2;
            }
            else
            {
                intSelectedTin = 3;
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

        public Task<CommunicationData> ReadTref()
        {
            var requestArray = HeaterDefaults.ReadTref();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadImax()
        {
            var requestArray = HeaterDefaults.ReadImax();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadCycles()
        {
            var requestArray = HeaterDefaults.ReadCycles();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadControl()
        {
            var requestArray = HeaterDefaults.ReadControl();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadSelect()
        {
            var requestArray = HeaterDefaults.ReadSelect();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadIntegralGain()
        {
            var requestArray = HeaterDefaults.ReadIntegralGain();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadProportionalGain()
        {
            var requestArray = HeaterDefaults.ReadProportionalGain();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadDerivativeGain()
        {
            var requestArray = HeaterDefaults.ReadDerivativeGain();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
