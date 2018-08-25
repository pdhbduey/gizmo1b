#include <LibThermistor1.h>

LibThermistor1::LibThermistor1(LibAdc* libAdc, int type, int units) :
    LibThermistor(libAdc, type, units)
{
    m_channelToAdcInMap[AIN_A] = LibAdc::CHANNEL_2;
    m_channelToAdcInMap[AIN_B] = LibAdc::CHANNEL_3;
    m_channelToAdcInMap[AIN_C] = LibAdc::CHANNEL_4;
    m_channelToAdcInMap[AIN_D] = LibAdc::CHANNEL_5;
}

// TEMP_AINx = 2.048 * Rt/(10,700 + Rt)
// Rt = 10,700 / (2.048 / TEMP_AINx - 1)
float LibThermistor1::convertVoltageToResistance(float ain)
{
    if (ain == 0 || ain == 2.048) {
        return 10000;
    }
    float rt = 10700 / (2.048 / ain - 1);
    return rt;
}
