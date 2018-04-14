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

        public Task<byte[]> SetStepMode(string selectedMode)
        {
            var requestArray = MotorDefaults.SetStepModeCommand(selectedMode);
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> SetDirection(string selectedDirection)
        {
            var requestArray = MotorDefaults.SetDirectionCommand(selectedDirection);
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> Home()
        {
            var requestArray = MotorDefaults.SetHomeCommand();
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

        public Task<byte[]> Reset()
        {
            var requestArray = MotorDefaults.SetResetCommand();
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

        public Task<byte[]> MoveRelative()
        {
            var requestArray = MotorDefaults.SetMoveRelativeCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }

        public Task<byte[]> MoveAbsolute()
        {
            var requestArray = MotorDefaults.SetMoveAbsoluteCommand();
            communication.WriteData(requestArray);
            var response = communication.ReadData();
            return response;
        }
    }
}
