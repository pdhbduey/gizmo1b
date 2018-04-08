// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    using DeviceCommunication;

    /// <summary>
    /// This class is responsible for managing the state of LED and switch A & B on board.
    /// </summary>
    public class LedModel : ILedModel
    {
        private IComCommunication communication;

        /// <summary>
        /// Creates an instance of the LedModel class.
        /// </summary>
        /// <param name="communication"> Com port communication class. </param>
        public LedModel(IComCommunication communication)
        {
            this.communication = communication;
        }

        /// <summary>
        /// Writes to LED request.
        /// </summary>
        /// <param name="request"> LED toggle status. </param>
        /// <returns> Task which will return LED register response. </returns>
        public Task<byte[]> SetLedCommand(string ledRequest)
        {
            var requestArray = LedDefaults.GetLedWriteCommand(ledRequest);
            communication.WriteData(requestArray);
            var data = communication.ReadData();
            return data;
        }
    }
}
