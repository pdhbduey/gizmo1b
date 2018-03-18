#include "libAdc.h"

//AD7689ACPZ
//8CH 16BIT ADC
// 98:MIBSPI5SOMI[0]:ADC_CNV
// MIBSPI1 group 0 (use HALCOGEN to configure):
// CLK frequency:
// word size:
// number of words:
// CLK polarity and phase:

LibAdc::LibAdc()
{
}

LibAdc::~LibAdc()
{
}

int LibAdc::read(int channel, float& value)
{
    int result = startConversion(channel);
    if (result != OKAY) {
        return result;
    }
    m_status = DONE;
    value = getResult();
    return OKAY;
}

int LibAdc::startConversion(int channel)
{
    switch (channel) {
    case CHANNEL_0:
    case CHANNEL_1:
    case CHANNEL_2:
    case CHANNEL_3:
    case CHANNEL_4:
    case CHANNEL_5:
        break;
    default:
        return ERROR_INVALID_CHANNEL;
    }
    m_status = DONE;
    return OKAY;
}

int LibAdc::getStatus()
{
    return m_status;
}

float LibAdc::getResult()
{
    return m_result;
}
