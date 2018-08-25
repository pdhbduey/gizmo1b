#include <LibThermistor.h>
#include <libMutex.h>
#include <LibThermistorCurves.h>

LibThermistor::LibThermistor(LibAdc* libAdc, int type, int units) :
    m_libAdc(*libAdc),
    m_libThermistorCurves(type, units),
    m_mutex(xSemaphoreCreateMutex())
{
}

int LibThermistor::setType(int type)
{
    LibMutex libMutex(m_mutex);
    return m_libThermistorCurves.setType(type);
}

int LibThermistor::getType()
{
    LibMutex libMutex(m_mutex);
    return m_libThermistorCurves.getType();
}

int LibThermistor::setUnits(int units)
{
    LibMutex libMutex(m_mutex);
    return m_libThermistorCurves.setUnits(units);
}

int LibThermistor::getUnits()
{
    LibMutex libMutex(m_mutex);
    return m_libThermistorCurves.getUnits();
}

int LibThermistor::readTemp(int channel, float& value)
{
    LibMutex libMutex(m_mutex);
    if (m_channelToAdcInMap.find(channel) == m_channelToAdcInMap.end()) {
        return ERROR_INVALID_TEMP_CHANNEL;
    }
    float ain;
    if (m_libAdc.read(m_channelToAdcInMap[channel], ain) != LibAdc::OKAY) {
        return ERROR_READ_TEMP_AIN_A + channel;
    }
    float rt = convertVoltageToResistance(ain);
    value    = m_libThermistorCurves.getTemperature(rt);
    return OKAY;
}

int LibThermistor::readTemp(int channel, uint32& value)
{
    LibMutex libMutex(m_mutex);
    if (m_channelToAdcInMap.find(channel) == m_channelToAdcInMap.end()) {
        return ERROR_INVALID_TEMP_CHANNEL;
    }
    uint32 ain;
    if (m_libAdc.read(m_channelToAdcInMap[channel], ain) != LibAdc::OKAY) {
        return ERROR_READ_TEMP_AIN_A + channel;
    }
    value = ain;
    return OKAY;
}
