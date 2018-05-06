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
        public bool SetDacCommand(float voltage, ref byte[] response)
        {
            var formattedVoltage = Helper.GetBigEndian(voltage);
            var request = DacDefaults.SetDacCommand(formattedVoltage);
            var status = communication.ProcessCommunicationRequest(request, ref response);
            return status;
        }

        /// <summary>
        /// Read DAC status.
        /// </summary>
        /// <returns> Returns the DAC status. </returns>
        public bool ReadDacStatusCommand(ref byte[] response)
        {
            var request = DacDefaults.ReadDacCommand();
            var status = communication.ProcessCommunicationRequest(request, ref response);
            return status;
        }
    }
}
