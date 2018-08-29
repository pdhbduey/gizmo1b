#include <LibTec1.h>

LibTec1::LibTec1(LibDac* libDac, LibAdc* libAdc, LibThermistor* libThermistor, const char* name) :
    LibTec(libDac,libAdc, libThermistor, name)
{
    m_adcChannelsMap[ISENSE] = LibAdc::CHANNEL_1;
    m_adcChannelsMap[VSENSE] = LibAdc::CHANNEL_0;
}

// TEC_ISENSE = (ISENSE * 0.008ohm * 20V/V + 2.5V) * 6.04K/9.05K
float LibTec1::toIsense(float voltage)
{
    float iSense = (voltage * 9.05 / 6.04 - 2.5) / (20.0 * 0.008);
    return iSense;
}

// TEC_VSENSE = VSENSE * 1.5K/21.5K + 2.5V
float LibTec1::toVsense(float voltage)
{
    float vSense = (voltage - 2.5) * 21.5 / 1.5;
    return vSense;
}
