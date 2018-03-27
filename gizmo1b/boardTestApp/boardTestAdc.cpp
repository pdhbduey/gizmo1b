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
        value = m_channel << ADC_CHANNEL_SHIFT;
        break;
    case ADC_STATUS:
        value = m_status <<  ADC_STATUS_SHIFT;
        break;
    case ADC_RESULT:
        value = *reinterpret_cast<uint32*>(&m_result); // NOTE: PC side may be LE
        break;
    }
    return OKAY;
}

int BoardTestAdc::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case ADC_STATUS:
    case ADC_RESULT:
        return ERROR_RO;
    case ADC_CONTROL:
        m_channel  = value >> ADC_CHANNEL_SHIFT;
        m_channel &= ADC_CHANNEL_MASK;
        if (value & (ADC_START_MASK << ADC_START_MASK_SHIFT)) {
            float value;
            m_status = m_libAdc.read(m_channel, value);
            if (m_status == LibAdc::OKAY) {
                m_result = value;
            }
        }
        break;
    }
    return OKAY;
}
