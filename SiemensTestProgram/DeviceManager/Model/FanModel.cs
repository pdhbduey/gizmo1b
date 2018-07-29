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
        public Task<CommunicationData> GetFanStatus()
        {
            var requestArray = FanDefaults.GetStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        /// <summary>
        /// Gets pwm period.
        /// </summary>
        public Task<CommunicationData> GetFanPwmPeriod(int channel)
        {
            var requestArray = FanDefaults.GetFanPeriodCommand(channel);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        /// <summary>
        /// Gets pwm duty cycle.
        /// </summary>
        public Task<CommunicationData> GetFanPwmDutyCycle(int channel)
        {
            var requestArray = FanDefaults.GetFanDutyCycleCommand(channel);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        /// <summary>
        /// Sets pwm one duty cycle.
        /// </summary>
        public Task<CommunicationData> SetFanPwmDutyCycle(int pwmChannel, int dutyCycle)
        {
            var requestArray = FanDefaults.SetFanDutyCycleCommand(pwmChannel, dutyCycle);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }

        /// <summary>
        /// Sets pwm period.
        /// </summary>
        public Task<CommunicationData> SetFanPwmPeriod(int pwmChannel, float period)
        {
            var requestArray = FanDefaults.SetFanPeriodCommand(pwmChannel, period);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }


        /// <summary>
        /// Gets sensor rpm.
        /// </summary>
        /// <returns> Value of sensor. </returns>
        public Task<CommunicationData> GetFanSensorRpm(int sensor)
        {
            var requestArray = FanDefaults.GetFanSensorRpmCommand(sensor);
            var status = communication.ProcessCommunicationRequest(requestArray);
            return status;
        }
    }
}
