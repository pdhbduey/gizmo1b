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
        public bool GetFanStatus(ref byte[] response)
        {
            var requestArray = FanDefaults.GetStatusCommand();
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        /// <summary>
        /// Gets pwm period.
        /// </summary>
        public bool GetFanPwmPeriod(int channel, ref byte[] response)
        {
            var requestArray = FanDefaults.GetFanPeriodCommand(channel);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        /// <summary>
        /// Gets pwm duty cycle.
        /// </summary>
        public bool GetFanPwmDutyCycle(int channel, ref byte[] response)
        {
            var requestArray = FanDefaults.GetFanDutyCycleCommand(channel);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        /// <summary>
        /// Sets pwm one duty cycle.
        /// </summary>
        public bool SetFanPwmDutyCycle(int pwmChannel, int dutyCycle, ref byte[] response)
        {
            var requestArray = FanDefaults.SetFanDutyCycleCommand(pwmChannel, dutyCycle);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }

        /// <summary>
        /// Sets pwm period.
        /// </summary>
        public bool SetFanPwmPeriod(int pwmChannel, float period, ref byte[] response)
        {
            var requestArray = FanDefaults.SetFanPeriodCommand(pwmChannel, period);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }


        /// <summary>
        /// Gets sensor rpm.
        /// </summary>
        /// <returns> Value of sensor. </returns>
        public bool GetFanSensorRpm(int sensor, ref byte[] response)
        {
            var requestArray = FanDefaults.GetFanSensorRpmCommand(sensor);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }
    }
}
