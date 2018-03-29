#include "boardTestDac.h"

BoardTestDac::BoardTestDac() :
    m_status(m_libDac.set(2.5))
{

}

BoardTestDac::~BoardTestDac()
{
}

int BoardTestDac::get(uint32 address, uint32& value)
{
    float v;
    switch (address) {
    default:
        return ERROR_ADDR;
    case DAC_VALUE:
        v = m_libDac.get();
        value = *reinterpret_cast<uint32*>(&v); // NOTE: PC side may be LE
        break;
    case DAC_STATUS:
        value = m_status;
        break;
    }
    return OKAY;
}

int BoardTestDac::set(uint32 address, uint32 value)
{
    float v;
    switch (address) {
    default:
        return ERROR_ADDR;
    case DAC_STATUS:
        return ERROR_RO;
    case DAC_VALUE:
        v = *reinterpret_cast<float*>(&value);
        m_status = m_libDac.set(v);
        break;
    }
    return OKAY;
}
