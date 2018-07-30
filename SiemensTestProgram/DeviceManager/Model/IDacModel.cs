// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using DeviceManager.DeviceCommunication;
    using System.Threading.Tasks;

    public interface IDacModel
    {
        /// <summary>
        /// Set DAC value.
        /// </summary>
        /// <param name="voltage"> Voltage value. </param>
        /// <returns> Returns the value from DAC set request. </returns>
        Task<CommunicationData> SetDacCommand(float voltage);

        /// <summary>
        /// Read DAC value.
        /// </summary>
        /// <returns> Returns the DAC value. </returns>
        Task<CommunicationData> GetDacValueCommand();

        /// <summary>
        /// Read DAC status.
        /// </summary>
        /// <returns> Returns the DAC status. </returns>
        Task<CommunicationData> ReadDacStatusCommand();
    }
}
