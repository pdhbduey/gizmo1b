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

        public bool SetRegisterValue(byte[] value, ref byte[] response)
        {
            var requestArray = MotorDefaults.SetRegisterValueCommand(value);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool SetRegisterAddress(string address, ref byte[] response)
        {
            var requestArray = MotorDefaults.SetRegisterAddressCommand(address);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool ReadRegisterValue(ref byte[] response)
        {
            var requestArray = MotorDefaults.ReadRegisterValueCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        //public Task<byte[]> SetStepMode(string selectedMode)
        //{
        //    var requestArray = MotorDefaults.SetStepModeCommand(selectedMode);
        //    communication.WriteData(requestArray);
        //    var response = communication.ReadData();
        //    return response;
        //}

        //public Task<byte[]> SetDirection(string selectedDirection)
        //{
        //    var requestArray = MotorDefaults.SetDirectionCommand(selectedDirection);
        //    communication.WriteData(requestArray);
        //    var response = communication.ReadData();
        //    return response;
        //}

        public bool Home(ref byte[] response)
        {
            var requestArray = MotorDefaults.SetHomeCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool Reset(ref byte[] response)
        {
            var requestArray = MotorDefaults.SetResetCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool Limp(ref byte[] response)
        {
            var requestArray = MotorDefaults.SetLimpCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool Energize(ref byte[] response)
        {
            var requestArray = MotorDefaults.SetEnergizeCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }


        public bool Initialize(ref byte[] response)
        {
            var requestArray = MotorDefaults.SetInitializeCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool GetMotorStatus(ref byte[] response)
        {
            var requestArray = MotorDefaults.GetMotorStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }


        public void SetRelativeMovePosition(int relativePosition, ref byte[] response)
        {
            var requestArray = MotorDefaults.SetMoveRelativePositionCommand(relativePosition);
            communication.ProcessCommunicationRequest(requestArray, ref response);
        }

        public void SetAbsoluteMovePosition(int absolutePosition, ref byte[] response)
        {
            var requestArray = MotorDefaults.SetMoveAbsolutePositionCommand(absolutePosition);
            communication.ProcessCommunicationRequest(requestArray, ref response);
        }

        public bool MotorControlMove(string selectedDirection, string selectedStepSize, string move, ref byte[] response)
        {
            var requestArray = MotorDefaults.SetControlMoveCommand(selectedDirection, selectedStepSize, move);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool Cycle(ref byte[] response)
        {
            var requestArray = MotorDefaults.CycleCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public bool Stop(ref byte[] response)
        {
            var requestArray = MotorDefaults.StopCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        //public Task<byte[]> MoveRelative()
        //{
        //    var requestArray = MotorDefaults.SetMoveRelativeCommand();
        //    communication.WriteData(requestArray);
        //    var response = communication.ReadData();

        //    return response;
        //}

        //public Task<byte[]> MoveAbsolute()
        //{
        //    var requestArray = MotorDefaults.SetMoveAbsoluteCommand();
        //    communication.WriteData(requestArray);
        //    var response = communication.ReadData();

        //    return response;
        //}

        public bool GetMotorPosition(ref byte[] response)
        {
            var requestArray = MotorDefaults.ReadPositionCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        public Task<CommunicationData> GetMotorPosition()
        {
            var requestArray = MotorDefaults.ReadPositionCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
        public Task<CommunicationData> GetMotorStatus()
        {
            var requestArray = MotorDefaults.GetMotorStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
