#include <BoardTestFault.h>

BoardTestFault::BoardTestFault(LibFault* libFault) :
    m_libFault(*libFault)
{
}

int BoardTestFault::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case FAULT_RESET:
        return ERROR_WO;
    case FAULT_STATE:
        {
            value = 0;
            bool isFault;
            m_libFault.getFault(LibFault::TEC_OCD_POS, isFault);
            value |= (isFault ? TEC_OCD_POS : 0);
            m_libFault.getFault(LibFault::TEC_OCD_NEG, isFault);
            value |= (isFault ? TEC_OCD_NEG : 0);
            bool isNtcPresent;
            m_libFault.getNtcPresent(LibFault::NTC1, isNtcPresent);
            if (isNtcPresent) {
                m_libFault.getFault(LibFault::OVERTEMP1, isFault);
                value |= (isFault ? OVERTEMP1 : 0);
            }
            m_libFault.getNtcPresent(LibFault::NTC2, isNtcPresent);
            if (isNtcPresent) {
                m_libFault.getFault(LibFault::OVERTEMP2, isFault);
                value |= (isFault ? OVERTEMP2 : 0);
            }
        }
        break;
    case FAULT_NTC_PRESENT:
        {
            value = 0;
            bool isNtcPresent;
            m_libFault.getNtcPresent(LibFault::NTC1, isNtcPresent);
            value |= (isNtcPresent ? NTC1_PRESENT : 0);
            m_libFault.getNtcPresent(LibFault::NTC2, isNtcPresent);
            value |= (isNtcPresent ? NTC2_PRESENT : 0);
        }
        break;
    }
    return OKAY;
}

int BoardTestFault::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case FAULT_STATE:
    case FAULT_NTC_PRESENT:
        return ERROR_RO;
    case FAULT_RESET:
        if (value & FAULT_RESET_MASK) {
            m_libFault.reset();
        }
        break;
    }
    return OKAY;
}
