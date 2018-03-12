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
    }
    return OKAY;
}

int BoardTestDac::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case DAC_VALUE:
        m_value = *reinterpret_cast<float*>(&value);
        m_libDac.set(m_value);
        break;
    }
    return OKAY;
}
