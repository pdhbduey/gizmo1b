// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    public interface IDacModel
    {
        /// <summary>
        /// Set DAC value.
        /// </summary>
        /// <param name="voltage"> Voltage value. </param>
        /// <returns> Returns the value from DAC set request. </returns>
        Task<byte[]> SetDacCommand(float voltage);

        /// <summary>
        /// Read DAC status.
        /// </summary>
        /// <returns> Returns the DAC status. </returns>
        Task<byte[]> ReadDacStatusCommand();
    }
}
