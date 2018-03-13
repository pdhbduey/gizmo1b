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
        value = ((m_channel + 1) << ADC_CHANNEL_SELECT_SHIFT)
                                                      & ADC_CHANNEL_SELECT_MASK;
        break;
    case ADC_STATUS:
        value = (m_libAdc.getStatus() << ADC_STATUS_SHIFT) &  ADC_STATUS_MASK;
        break;
    case ADC_RESULT:
        float result = m_libAdc.getResult();
        value = *reinterpret_cast<uint32*>(&result); // NOTE: PC side may be LE
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
        channel = (value & ADC_CHANNEL_SELECT_MASK) >> ADC_CHANNEL_SELECT_SHIFT;
        if (channel) {
            m_channel = channel;
        }
        if (value & START_ADC_MASK) {
            m_libAdc.startConversion(m_channel);
        }
        break;
    }
    return OKAY;
}
