#include "libAdc.h"

bool LibAdc::s_isInitialized = false;

LibAdc::LibAdc() :
    m_status(IDLE)
{
    if (!s_isInitialized) {
        s_isInitialized = true;
    }
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
