// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    using DeviceCommunication;

    public class FanModel : IFanModel
    {
        private IComCommunication communication;

        public FanModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        /// <summary>
        /// Gets the status of the fan.
        /// </summary>
        /// <returns> Task that returns fan status </returns>
        public Task<byte[]> GetFanStatus()
        {
            var requestArray = FanDefaults.GetStatusCommand();
            communication.WriteData(requestArray);
            var status = communication.ReadData();
            return status;
        }

        /// <summary>
        /// Gets pwm period.
        /// </summary>
        public Task<byte[]> GetFanPwmPeriod(int channel)
        {
            var requestArray = FanDefaults.GetFanPeriodCommand(channel);
            communication.WriteData(requestArray);
            var status = communication.ReadData();
            return status;
        }

        /// <summary>
        /// Gets pwm duty cycle.
        /// </summary>
        public Task<byte[]> GetFanPwmDutyCycle(int channel)
        {
            var requestArray = FanDefaults.GetFanDutyCycleCommand(channel);
            communication.WriteData(requestArray);
            var status = communication.ReadData();
            return status;
        }

        /// <summary>
        /// Sets pwm one duty cycle.
        /// </summary>
        public Task<byte[]> SetFanPwmDutyCycle(int pwmChannel, int dutyCycle)
        {
            var requestArray = FanDefaults.SetFanDutyCycleCommand(pwmChannel, dutyCycle);
            communication.WriteData(requestArray);
            var status = communication.ReadData();
            return status;
        }

        /// <summary>
        /// Sets pwm period.
        /// </summary>
        public Task<byte[]> SetFanPwmPeriod(int pwmChannel, int period)
        {
            var requestArray = FanDefaults.SetFanPeriodCommand(pwmChannel, period);
            communication.WriteData(requestArray);
            var status = communication.ReadData();
            return status;
        }


        /// <summary>
        /// Gets sensor rpm.
        /// </summary>
        /// <returns> Value of sensor. </returns>
        public Task<byte[]> GetFanSensorRpm(int sensor)
        {
            var requestArray = FanDefaults.GetFanSensorRpmCommand(sensor);
            communication.WriteData(requestArray);
            var status = communication.ReadData();
            return status;
        }
    }
}
