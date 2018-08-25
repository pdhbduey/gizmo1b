#include <BoardTestAdc.h>

BoardTestAdc::BoardTestAdc(LibAdc* libAdc) :
    m_libAdc(*libAdc)
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
        value = *reinterpret_cast<uint32*>(&m_result);
        break;
    case ADC_CHANNEL_0:
    case ADC_CHANNEL_1:
    case ADC_CHANNEL_2:
    case ADC_CHANNEL_3:
    case ADC_CHANNEL_4:
    case ADC_CHANNEL_5:
    case ADC_CHANNEL_6:
    case ADC_CHANNEL_7:
        {
            float voltage;
            int channel = address - ADC_CHANNEL_0;
            m_status = m_libAdc.read(channel, voltage);
            if (m_status == LibAdc::OKAY) {
                value = *reinterpret_cast<uint32*>(&voltage);
            }
        }
        break;
    }
    return OKAY;
}

int BoardTestAdc::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case ADC_CHANNEL_0:
    case ADC_CHANNEL_1:
    case ADC_CHANNEL_2:
    case ADC_CHANNEL_3:
    case ADC_CHANNEL_4:
    case ADC_CHANNEL_5:
    case ADC_CHANNEL_6:
    case ADC_CHANNEL_7:
    case ADC_STATUS:
    case ADC_RESULT:
        return ERROR_RO;
    case ADC_CONTROL:
        m_channel  = value >> ADC_CHANNEL_SHIFT;
        m_channel &= ADC_CHANNEL_MASK;
        if (value & (ADC_START_MASK << ADC_START_MASK_SHIFT)) {
            float voltage;
            m_status = m_libAdc.read(m_channel, voltage);
            if (m_status == LibAdc::OKAY) {
                m_result = voltage;
            }
        }
        break;
    }
    return OKAY;
}
