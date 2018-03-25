#include "boardTestDac.h"

BoardTestDac::BoardTestDac()
{
}

BoardTestDac::~BoardTestDac()
{
}

int BoardTestDac::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case DAC_VALUE:
        value = *reinterpret_cast<uint32*>(&m_value); // NOTE: PC side may be LE
        break;
    case DAC_STATUS:
        value = m_status;
        break;
    }
    return OKAY;
}

int BoardTestDac::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case DAC_STATUS:
        return ERROR_RO;
    case DAC_VALUE:
        float v = *reinterpret_cast<float*>(&value);
        m_status = m_libDac.set(v);
        if (m_status == LibDac::OKAY) {
            m_value = v;
        }
        break;
    }
    return OKAY;
}
