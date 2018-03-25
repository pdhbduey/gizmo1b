#include "boardTestAdc.h"

BoardTestAdc::BoardTestAdc()
{
}

BoardTestAdc::~BoardTestAdc()
{
}

int BoardTestAdc::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case ADC_CONTROL:
        value = ((m_channel + 1) & ADC_CHANNEL) << ADC_CHANNEL_SHIFT;
        break;
    case ADC_STATUS:
        value = (m_status & ADC_STATUS) <<  ADC_STATUS_SHIFT;
        break;
    case ADC_RESULT:
        value = *reinterpret_cast<uint32*>(&m_result); // NOTE: PC side may be LE
        break;
    }
    return OKAY;
}

int BoardTestAdc::set(uint32 address, uint32 value)
{
    int channel;
    switch (address) {
    default:
        return ERROR_ADDR;
    case ADC_STATUS:
    case ADC_RESULT:
        return ERROR_RO;
    case ADC_CONTROL:
        channel = (value >> ADC_CHANNEL_SHIFT) & ADC_CHANNEL;
        if (channel) {
            m_channel = channel - 1;
            m_status = m_libAdc.setChannel(m_channel);
            if (m_status != LibAdc::OKAY) {
                return OKAY;
            }
        }
        if ((value >> ADC_START_SHIFT) & ADC_START) {
            float value;
            m_status = m_libAdc.read(value);
            if (m_status == LibAdc::OKAY) {
                m_result = value;
            }
        }
        break;
    }
    return OKAY;
}
