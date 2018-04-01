// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    using DeviceCommunication;
    using Common;

    public class DacModel : IDacModel   
    {
        private IComCommunication communication;

        public DacModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        /// <summary>
        /// Writes data through the comp port.
        /// </summary>
        /// <param name="request"> Sends a request. </param>
        /// <returns> Task which returns byte array of response when completed. </returns>
        public Task<byte[]> WriteData(float voltage)
        {
            var formattedVoltage = Helper.GetBigEndian(voltage);
            var request = ComCommands.GetDacCommand(ComCommands.REGISTER_WRITE, formattedVoltage);
            communication.WriteData(request);
            return communication.ReadData();
        }
    }
}
