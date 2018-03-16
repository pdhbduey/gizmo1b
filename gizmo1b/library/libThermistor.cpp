#include "libThermistor.h"

LibThermistor::LibThermistor()
{
}

LibThermistor::~LibThermistor()
{
}

int LibThermistor::readTemp(int channel, float& value)
{
    int result = startReading(channel);
    if (result != OKAY) {
        return result;
    }
    m_status = DONE;
    value = getResult();
    return OKAY;
}

int LibThermistor::startReading(int channel)
{
    switch (channel) {
    case AIN_A:
    case AIN_B:
    case AIN_C:
    case AIN_D:
        break;
    default:
        return ERROR_INVALID_TEMP_CHANNEL;
    }
    m_status = DONE;
    return OKAY;
}

int LibThermistor::getStatus()
{
    return m_status;
}

float LibThermistor::getResult()
{
    return m_result;
}
