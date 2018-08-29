#include <BoardTestAdcIV.h>

BoardTestAdcIV::BoardTestAdcIV(LibAdc* libAdc) :
    m_libAdc(*libAdc)
{
}

int BoardTestAdcIV::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case ADC_IV_STATUS:
        value = m_status;
        break;
    case ADC_IV_CHANNEL_0:
    case ADC_IV_CHANNEL_1:
        {
            float voltage;
            int channel = address - ADC_IV_CHANNEL_0;
            m_status = m_libAdc.read(channel, voltage);
            if (m_status == LibAdc::OKAY) {
                value = *reinterpret_cast<uint32*>(&voltage);
            }
        }
        break;
    }
    return OKAY;
}
