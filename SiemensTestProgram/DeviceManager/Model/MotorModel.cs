// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using Common;
    using DeviceCommunication;
    using System.Threading.Tasks;

    public class MotorModel : IMotorModel
    {
        private IComCommunication communication;

        public MotorModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<CommunicationData> SetRegisterValue(byte[] value)
        {
            var requestArray = MotorDefaults.SetRegisterValueCommand(value);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetRegisterAddress(string address)
        {
            var requestArray = MotorDefaults.SetRegisterAddressCommand(address);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> ReadRegisterValue()
        {
            var requestArray = MotorDefaults.ReadRegisterValueCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> Home()
        {
            var requestArray = MotorDefaults.SetHomeCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> Reset()
        {
            var requestArray = MotorDefaults.SetResetCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> Limp()
        {
            var requestArray = MotorDefaults.SetLimpCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> Energize()
        {
            var requestArray = MotorDefaults.SetEnergizeCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> Initialize()
        {
            var requestArray = MotorDefaults.SetInitializeCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> GetMotorStatus()
        {
            var requestArray = MotorDefaults.GetMotorStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        public Task<CommunicationData> SetRelativeMovePosition(int relativePosition)
        {
            var requestArray = MotorDefaults.SetMoveRelativePositionCommand(relativePosition);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> SetAbsoluteMovePosition(int absolutePosition)
        {
            var requestArray = MotorDefaults.SetMoveAbsolutePositionCommand(absolutePosition);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> MotorControlMove(string selectedDirection, string selectedStepSize, string move)
        {
            var requestArray = MotorDefaults.SetControlMoveCommand(selectedDirection, selectedStepSize, move);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> Cycle()
        {
            var requestArray = MotorDefaults.CycleCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> Stop()
        {
            var requestArray = MotorDefaults.StopCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        public Task<CommunicationData> GetMotorPosition()
        {
            var requestArray = MotorDefaults.ReadPositionCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
