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
        public bool SetLedCommand(string ledRequest, ref byte[] response)
        {
            var requestArray = LedDefaults.GetLedWriteCommand(ledRequest);
            var status = communication.ProcessCommunicationRequest(requestArray, ref response);
            return status;
        }
    }
}
