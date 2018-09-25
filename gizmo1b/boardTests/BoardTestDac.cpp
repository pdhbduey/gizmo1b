#include <BoardTestDac.h>

BoardTestDac::BoardTestDac(LibDac* libDac, LibTec* libTec) :
    m_libDac(*libDac),
    m_libTec(*libTec)
{
    m_status = m_libDac.set((float) 2.5);
}

int BoardTestDac::get(uint32 address, uint32& value)
{
    float v;
    switch (address) {
    default:
        return ERROR_ADDR;
    case DAC_VALUE:
        v = m_libDac.get();
        value = *reinterpret_cast<uint32*>(&v);
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
        if (!m_libTec.isEnabled()) {
            float v = *reinterpret_cast<float*>(&value);
            m_status = m_libDac.set(v);
        }
        break;
    }
    return OKAY;
}
