#include "boardTestFault.h"

BoardTestFault::BoardTestFault()
{
}

BoardTestFault::~BoardTestFault()
{
}

int BoardTestFault::get(uint32 address, uint32& value)
{
    bool isFault;
    bool isNtcPresent;
    switch (address) {
    default:
        return ERROR_ADDR;
    case FAULT_RESET:
        return ERROR_WO;
    case FAULT_STATE:
        value = 0;
        m_libFault.readState(LibFault::TEC_OCD_POS, isFault);
        value |= isFault ? TEC_OCD_POS_STATUS_MASK : 0;
        m_libFault.readState(LibFault::TEC_OCD_NEG, isFault);
        value |= isFault ? TEC_OCD_NEG_STATUS_MASK : 0;
        m_libFault.readState(LibFault::OVERTEMP1, isFault);
        value |= isFault ? OVERTEMP1_STATUS_MASK : 0;
        m_libFault.readState(LibFault::OVERTEMP2, isFault);
        value |= isFault ? OVERTEMP2_STATUS_MASK : 0;
        m_libFault.readState(LibFault::NTC1, isNtcPresent);
        value |= isNtcPresent ? NTC1_PRESENT_MASK : 0;
        m_libFault.readState(LibFault::NTC2, isNtcPresent);
        value |= isNtcPresent ? NTC2_PRESENT_MASK : 0;
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
        return ERROR_RO;
    case FAULT_RESET:
        if (value & FAULT_RESET_MASK) {
            m_libFault.reset();
        }
        break;
    }
    return OKAY;
}
