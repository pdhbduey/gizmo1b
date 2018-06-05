// <--------------------------------------------- Gizmo1B Test Program --------------------------------------------->

namespace Common
{
    using System;
    using System.Collections.Generic;
    using System.IO;

    /// <summary>
    /// This is a helper class for doing generic computations.
    /// </summary>
    public static class Helper
    {
        /// <summary>
        /// Sees if data from two byte arrays are equal.
        /// </summary>
        /// <param name="first"> First array. </param>
        /// <param name="second"> Second array. </param>
        /// <returns> True if values in array are equal, false otherwise. </returns>
        public static bool AreByteArraysEqual(byte[] first, byte[] second)
        {
            if (first.Length != second.Length)
            {
                return false;
            }

            for (var index = 0; index < first.Length; index++)
            {
                if (first[index] != second[index])
                {
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Checks if data bit is set.
        /// </summary>
        /// <param name="dataByte"> Data byte </param>
        /// <param name="position"> Position of bit to check status </param>
        /// <returns> True if the bit at the position is set to 1, false otherwise </returns>
        public static bool IsBitSet(byte dataByte, int position, int value = 1)
        {
            return ((dataByte >> position) & value) != 0;
        } 

        public static byte IntegerToByte(int num)
        {
            return Convert.ToByte(num);
        }

        public static byte[] GetBigEndian(float value)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);
            return bytes;
        }

        public static float GetFloatFromBigEndian(byte[] bytes)
        {
            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes); // We have to reverse
            return BitConverter.ToSingle(bytes, 0);
        }

        public static int GetIntFromBigEndian(byte[] bytes)
        {
            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes); // We have to reverse
            return BitConverter.ToInt32(bytes, 0);
        }

        public static int GetIntFromLittleEndian(byte[] bytes)
        {
            if (!BitConverter.IsLittleEndian)
                Array.Reverse(bytes); // We have to reverse
            return BitConverter.ToInt32(bytes, 0);
        }

        public static byte[] ConvertIntToByteArray(int value)
        {
            byte[] bytes = BitConverter.GetBytes(value);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);
            return bytes;
        }

        public static byte[] ConvertStringToByteArray(String hex)
        {
            

            if (hex.Length % 2 != 0)
            {
                hex = "0" + hex;
            }

            var numberCharacters = hex.Length;
            byte[] bytes = new byte[numberCharacters / 2];
            for (int i = 0; i < numberCharacters; i += 2)
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            return bytes;
        }

        public class FileData
        {
            public List<int> sampleTimes;
            public List<float> sampleValueFirst;

            public FileData()
            {
                sampleTimes = new List<int>();
                sampleValueFirst = new List<float>();
            }

            public void AddSample(int sampleTime, float firstValue)
            {
                sampleTimes.Add(sampleTime);
                sampleValueFirst.Add(firstValue);
            }
        }

        public static FileData GetCsvData(string filePath)
        {
            var fileData = new FileData();

            using (var reader = new StreamReader(filePath))
            {
                // Reads the header
                reader.ReadLine();
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();
                    var values = line.Split(',');

                    var time = Int32.Parse(values[0]); 
                    var firstValue = float.Parse(values[1]);

                    fileData.AddSample(time, firstValue);
                }
            }

            return fileData;
        }
    }
}
