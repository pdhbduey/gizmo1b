// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceManager.DeviceCommunication;
    using System.Threading.Tasks;

    public interface IFanModel
    {
        /// <summary>
        /// Gets the status of the fan.
        /// </summary>
        /// <returns> Task that returns fan status </returns>
        Task<CommunicationData> GetFanStatus();

        /// <summary>
        /// Sets pwm duty cycle.
        /// </summary>
        Task<CommunicationData> SetFanPwmDutyCycle(int pwmChannel, int dutyCycle);


        /// <summary>
        /// Sets pwm period.
        /// </summary>
        Task<CommunicationData> SetFanPwmPeriod(int pwmChannel, float period);

        /// <summary>
        /// Gets pwm period.
        /// </summary>
        Task<CommunicationData> GetFanPwmPeriod(int channel);

        /// <summary>
        /// Gets pwm duty cycle.
        /// </summary>
        Task<CommunicationData> GetFanPwmDutyCycle(int channel);


        /// <summary>
        /// Gets sensor rpm.
        /// </summary>
        /// <returns> Value of sensor. </returns>
        Task<CommunicationData> GetFanSensorRpm(int sensor);
    }
}
