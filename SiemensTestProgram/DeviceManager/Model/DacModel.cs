// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    using Common;
    using DeviceCommunication;

    public class DacModel : IDacModel   
    {
        private IComCommunication communication;

        public DacModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        /// <summary>
        /// Set DAC value.
        /// </summary>
        /// <param name="voltage"> Voltage value. </param>
        /// <returns> Returns the value from DAC set request. </returns>
        public Task<byte[]> SetDacCommand(float voltage)
        {
            var formattedVoltage = Helper.GetBigEndian(voltage);
            var request = DacDefaults.SetDacCommand(formattedVoltage);
            communication.WriteData(request);
            return communication.ReadData();
        }

        /// <summary>
        /// Read DAC status.
        /// </summary>
        /// <returns> Returns the DAC status. </returns>
        public Task<byte[]> ReadDacStatusCommand()
        {
            var request = DacDefaults.ReadDacCommand();
            communication.WriteData(request);
            return communication.ReadData();
        }
    }
}
