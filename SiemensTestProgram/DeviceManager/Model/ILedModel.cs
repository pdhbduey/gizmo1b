// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager.Model
{
    using System.Threading.Tasks;

    /// <summary>
    /// Interface for LedModel class.
    /// </summary>
    public interface ILedModel
    {
        /// <summary>
        /// Writes to LED request.
        /// </summary>
        /// <param name="request"> LED toggle status. </param>
        /// <returns> Task which will return LED register response. </returns>
        Task<byte[]> SetLedCommand(string ledRequest);
    }
}
