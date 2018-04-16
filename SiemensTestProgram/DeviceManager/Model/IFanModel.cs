// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    public interface IFanModel
    {
        /// <summary>
        /// Gets the status of the fan.
        /// </summary>
        /// <returns> Task that returns fan status </returns>
        Task<byte[]> GetFanStatus();

        /// <summary>
        /// Sets pwm duty cycle.
        /// </summary>
        Task<byte[]> SetFanPwmDutyCycle(int pwmChannel, int dutyCycle);


        /// <summary>
        /// Sets pwm period.
        /// </summary>
        Task<byte[]> SetFanPwmPeriod(int pwmChannel, int period);

        /// <summary>
        /// Gets pwm period.
        /// </summary>
        Task<byte[]> GetFanPwmPeriod(int channel);

        /// <summary>
        /// Gets pwm duty cycle.
        /// </summary>
        Task<byte[]> GetFanPwmDutyCycle(int channel);


        /// <summary>
        /// Gets sensor rpm.
        /// </summary>
        /// <returns> Value of sensor. </returns>
        Task<byte[]> GetFanSensorRpm(int sensor);
    }
}
