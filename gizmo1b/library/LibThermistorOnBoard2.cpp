#include <LibThermistorOnBoard2.h>
#include <LibThermistorCurves.h>

LibThermistorOnBoard2::LibThermistorOnBoard2(LibAdc* libAdc) :
    LibThermistor(libAdc, LibThermistorCurves::NTCG163JF103FT1, LibThermistorCurves::CELSIUS)
{
    m_channelToAdcInMap[INA1] = LibAdc::CHANNEL_4;
    m_channelToAdcInMap[INA2] = LibAdc::CHANNEL_5;
    m_channelToAdcInMap[INB1] = LibAdc::CHANNEL_6;
    m_channelToAdcInMap[INB2] = LibAdc::CHANNEL_7;
}

// TEMP_AINx = 5.0 * Rt/(10,700 + Rt)
// Rt = 10,700 / (5.0 / TEMP_INx - 1)
float LibThermistorOnBoard2::convertVoltageToResistance(float ain)
{
    if (ain == 0 || ain == 5.0) {
        return 10000;
    }
    float rt = 10700 / (5.0 / ain - 1);
    return rt;
}
