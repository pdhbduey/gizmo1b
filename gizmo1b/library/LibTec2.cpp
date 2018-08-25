#include <LibTec2.h>

LibTec2::LibTec2(LibDac* libDac, LibAdc* libAdc, LibThermistor* libThermistor, const char* name) :
    LibTec(libDac,libAdc, libThermistor, name)
{
    m_adcChannelsMap[ISENSE] = LibAdc::CHANNEL_0;
    m_adcChannelsMap[VSENSE] = LibAdc::CHANNEL_1;
}

// TEC_ISENSE = (ISENSE * 0.006ohm * 20V/V + 2.5V) * 6.04K/9.05K
float LibTec2::toIsense(float voltage)
{
    float iSense = (voltage * 9.05 / 6.04 - 2.5) / (20.0 * 0.006);
    return iSense;
}

// TEC_VSENSE = VSENSE * 1.5K/21.5K + 2.5V
float LibTec2::toVsense(float voltage)
{
    float vSense = (voltage - 2.5) * 21.5 / 1.5;
    return vSense;
}
