#include <LibThermistor2.h>

LibThermistor2::LibThermistor2(LibAdc* libAdc, int type, int units) :
    LibThermistor(libAdc, type, units)
{
    m_channelToAdcInMap[AIN_A] = LibAdc::CHANNEL_0;
    m_channelToAdcInMap[AIN_B] = LibAdc::CHANNEL_1;
    m_channelToAdcInMap[AIN_C] = LibAdc::CHANNEL_2;
    m_channelToAdcInMap[AIN_D] = LibAdc::CHANNEL_3;
}

// TEMP_AINx = 4.096 * Rt/(10,700 + Rt)
// Rt = 10,700 / (4.096 / TEMP_AINx - 1)
float LibThermistor2::convertVoltageToResistance(float ain)
{
    if (ain == 0 || ain == 4.096) {
        return 10000;
    }
    float rt = 10700 / (4.096 / ain - 1);
    return rt;
}
