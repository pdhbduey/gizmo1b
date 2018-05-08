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
        bool GetFanStatus(ref byte[] response);

        /// <summary>
        /// Sets pwm duty cycle.
        /// </summary>
        bool SetFanPwmDutyCycle(int pwmChannel, int dutyCycle, ref byte[] response);


        /// <summary>
        /// Sets pwm period.
        /// </summary>
        bool SetFanPwmPeriod(int pwmChannel, float period, ref byte[] response);

        /// <summary>
        /// Gets pwm period.
        /// </summary>
        bool GetFanPwmPeriod(int channel, ref byte[] response);

        /// <summary>
        /// Gets pwm duty cycle.
        /// </summary>
        bool GetFanPwmDutyCycle(int channel, ref byte[] response);


        /// <summary>
        /// Gets sensor rpm.
        /// </summary>
        /// <returns> Value of sensor. </returns>
        bool GetFanSensorRpm(int sensor, ref byte[] response);
    }
}
