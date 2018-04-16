#ifndef _BOARD_TEST_FAULT_H_
#define _BOARD_TEST_FAULT_H_

#include "boardTest.h"
#include "libFault.h"

class BoardTestFault: public BoardTest
{
public:
    BoardTestFault();
    virtual ~BoardTestFault();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
    enum Faults {
        TEC_OCD_POS = (uint32)((uint32)1U << 0U),
        TEC_OCD_NEG = (uint32)((uint32)1U << 1U),
        OVERTEMP1   = (uint32)((uint32)1U << 2U),
        OVERTEMP2   = (uint32)((uint32)1U << 3U),
    };
    enum NtcPresent {
        NTC1_PRESENT = (uint32)((uint32)1U << 0U),
        NTC2_PRESENT = (uint32)((uint32)1U << 1U),
    };
    enum FaultReset {
        FAULT_RESET_MASK = 1 << 0,
    };
private:
    LibFault m_libFault;
    uint32 m_status;
};

#endif // _BOARD_TEST_FAULT_H_
