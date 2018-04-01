// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace DeviceManager
{
    using Common;
    using System;
    using System.Collections.Generic;
    using System.IO;

    /// <summary>
    /// General functions for command processing  
    /// </summary>
    public static class DataHelper
    {
        // Response string
        private const string unknownResponse = "Unknown Response";
        private const string invalidResponseFormat = "Invalid Response Format";
        private const string standardError = "Error";
        public const string clearMessage = "";

        // Commands
        public const byte REGISTER_READ = 0x01;
        public const byte REGISTER_WRITE = 0x02;

        // ON/OFF 
        public const byte LED_RED_ON = 0x01;
        public const byte LED_RED_OFF = 0x02;
        public const byte LED_GREEN_ON = 0x04;
        public const byte LED_GREEN_OFF = 0x08;

        // READ STATUS
        public const byte LED_A_STATUS = 0x01;
        public const byte LED_B_STATUS = 0x02;

        // Message Status
        public static Dictionary<byte, string> MessageStatus = new Dictionary<byte, string>()
        {
            { 0x00, "Message Received: "},
            { 0x01, "Error Unknown Command: "},
            { 0x02, "Error Unknown Register Address: "},
            { 0x03, "Error Read-Only: "},
            { 0x04, "Error Write-Only: "}
        };

        // Switch Status
        public static Dictionary<byte, string> SwitchStatus = new Dictionary<byte, string>()
        {
            { 0x03, "Switch A & B Released"},
            { 0x02, "Switch A Pressed"},
            { 0x01, "Switch B Pressed"},
            { 0x00, "Switch A & B Pressed"},
        };

        /// <summary>
        /// Parses given message to byte array
        /// </summary>
        /// <param name="message"></param>
        /// <returns> Byte array from message </returns>
        public static byte[] ParseStringToByteArray(string message)
        {
            if (string.IsNullOrEmpty(message))
            {
                return new byte[0];
            }

            // Removes 0x format and spaces
            message = message.Replace("0x", "");
            message = message.Replace(" ", "");
            
            // Converts string message to byte array
            var outputLength = message.Length / 2;
            var output = new byte[outputLength];
            using (var sr = new StringReader(message))
            {
                for (var i = 0; i < outputLength; i++)
                    output[i] = Convert.ToByte(new string(new char[2] { (char)sr.Read(), (char)sr.Read() }), 16);
            }
            return output;
        }

        public static string ProcessCommandResponse(byte[] response, ComCommands.CommandType commandType)
        {
            switch(commandType)
            {
                case ComCommands.CommandType.LED:
                    return ProcessLedResponse(response);
                default:
                    return standardError;
            }
        }

        private static string ProcessLedResponse(byte[] response)
        {
            string msg = "";
            string status;
            MessageStatus.TryGetValue(response[0], out status);

            if (status == null)
            {
                return unknownResponse;
            }

            msg += status;

            var registerData = response[4];
            if (Helper.IsBitSet(registerData, 0))
            {
                msg += "LED Red ON";
            }
            else if (Helper.IsBitSet(registerData, 1))
            {
                msg += "LED Red OFF";
            }
            else if (Helper.IsBitSet(registerData, 2))
            {
                msg += "LED Green ON";
            }
            else if (Helper.IsBitSet(registerData, 3))
            {
                msg += "LED Green OFF";
            }
            else
            {
                return unknownResponse;
            }

            return msg;
        }
    }
}
