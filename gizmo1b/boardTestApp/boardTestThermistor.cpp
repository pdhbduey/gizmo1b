#include "boardTestThermistor.h"

BoardTestThermistor::BoardTestThermistor()
{
}

BoardTestThermistor::~BoardTestThermistor()
{
}

int BoardTestThermistor::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case THERMISTOR_CONTROL:
        value = ((m_channel + 1) << THERMISTOR_CHANNEL_SELECT_SHIFT)
                                               & THERMISTOR_CHANNEL_SELECT_MASK;
        break;
    case THERMISTOR_STATUS:
        value = (m_libThermistor.getStatus() << THERMISTOR_STATUS_SHIFT)
                                                      &  THERMISTOR_STATUS_MASK;
        break;
    case THERMISTOR_RESULT:
        {
            float result = m_libThermistor.getResult();
            value = *reinterpret_cast<uint32*>(&result); // NOTE: PC side may be LE
        }
        break;
    }
    return OKAY;
}

int BoardTestThermistor::set(uint32 address, uint32 value)
{
    int channel;
    switch (address) {
    default:
        return ERROR_ADDR;
    case THERMISTOR_STATUS:
    case THERMISTOR_RESULT:
        return ERROR_RO;
    case THERMISTOR_CONTROL:
        channel = (value & THERMISTOR_CHANNEL_SELECT_MASK)
                                             >> THERMISTOR_CHANNEL_SELECT_SHIFT;
        if (channel) {
            m_channel = channel;
        }
        if (value & START_READING_MASK) {
            m_libThermistor.startReading(m_channel);
        }
        break;
    }
    return OKAY;
}
