// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using System.Collections.Generic;

    /// <summary>
    /// General functions for command processing  
    /// </summary>
    public static class DataHelper
    {
        // Register read/write 
        public const byte REGISTER_READ = 0x01;
        public const byte REGISTER_WRITE = 0x02;

        // Message Status
        public static Dictionary<byte, string> MessageStatus = new Dictionary<byte, string>()
        {
            { 0x00, "Message Received: "},
            { 0x01, "Error Unknown Command: "},
            { 0x02, "Error Unknown Register Address: "},
            { 0x03, "Error Read-Only: "},
            { 0x04, "Error Write-Only: "}
        };
    }
}
