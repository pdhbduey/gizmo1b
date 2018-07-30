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
        public Task<CommunicationData> SetDacCommand(float voltage)
        {
            var formattedVoltage = Helper.GetBigEndian(voltage);
            var request = DacDefaults.SetDacCommand(formattedVoltage);
            var status = communication.ProcessCommunicationRequest(request);
            return status;
        }

        /// <summary>
        /// Read DAC value.
        /// </summary>
        /// <returns> Returns the DAC value. </returns>
        public Task<CommunicationData> GetDacValueCommand()
        {
            var request = DacDefaults.ReadDacValueCommand();
            var status = communication.ProcessCommunicationRequest(request);
            return status;
        }

        /// <summary>
        /// Read DAC status.
        /// </summary>
        /// <returns> Returns the DAC status. </returns>
        public Task<CommunicationData> ReadDacStatusCommand()
        {
            var request = DacDefaults.ReadDacCommand();
            var status = communication.ProcessCommunicationRequest(request);
            return status;
        }
    }
}
