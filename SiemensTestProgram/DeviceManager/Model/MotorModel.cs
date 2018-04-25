// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceCommunication;
    using System.Threading.Tasks;

    public class MotorModel : IMotorModel
    {
        private IComCommunication communication;

        public MotorModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        public Task<byte[]> SetRegisterValue(byte[] value)
        {
            var requestArray = MotorDefaults.SetRegisterValueCommand(value);
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> SetRegisterAddress(string address)
        {
            var requestArray = MotorDefaults.SetRegisterAddressCommand(address);
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> ReadRegisterValue()
        {
            var requestArray = MotorDefaults.ReadRegisterValueCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
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

        public Task<byte[]> Home()
        {
            var requestArray = MotorDefaults.SetHomeCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> Reset()
        {
            var requestArray = MotorDefaults.SetResetCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> Limp()
        {
            var requestArray = MotorDefaults.SetLimpCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> Energize()
        {
            var requestArray = MotorDefaults.SetEnergizeCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }


        public Task<byte[]> Initialize()
        {
            var requestArray = MotorDefaults.SetInitializeCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public byte[] GetMotorStatus()
        {
            var requestArray = MotorDefaults.GetMotorStatusCommand();
            communication.WriteData(requestArray);
            return communication.ReadData().Result;
        }

        public void SetRelativeMovePosition(int relativePosition)
        {
            var requestArray = MotorDefaults.SetMoveRelativePositionCommand(relativePosition);
            communication.WriteData(requestArray);
            communication.ReadData().Wait();
        }

        public void SetAbsoluteMovePosition(int absolutePosition)
        {
            var requestArray = MotorDefaults.SetMoveAbsolutePositionCommand(absolutePosition);
            communication.WriteData(requestArray);
            communication.ReadData().Wait();
        }

        public Task<byte[]> MotorControlMove(string selectedDirection, string selectedStepSize, string move)
        {
            var requestArray = MotorDefaults.SetControlMoveCommand(selectedDirection, selectedStepSize, move);
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> Cycle()
        {
            var requestArray = MotorDefaults.CycleCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> Stop()
        {
            var requestArray = MotorDefaults.StopCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
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

        public Task<byte[]> GetMotorPosition()
        {
            var requestArray = MotorDefaults.ReadPositionCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();

            return response;
        }
    }
}
