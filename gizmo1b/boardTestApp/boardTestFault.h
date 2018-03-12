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
    enum Masks {
        NTC1_PRESENT_MASK       = (uint32)((uint32)1U << 0U),
        NTC2_PRESENT_MASK       = (uint32)((uint32)1U << 1U),
        TEC_OCD_POS_STATUS_MASK = (uint32)((uint32)1U << 2U),
        TEC_OCD_NEG_STATUS_MASK = (uint32)((uint32)1U << 3U),
        OVERTEMP1_STATUS_MASK   = (uint32)((uint32)1U << 4U),
        OVERTEMP2_STATUS_MASK   = (uint32)((uint32)1U << 5U),
        FAULT_RESET_MASK        = (uint32)((uint32)1U << 0U),
    };
private:
    LibFault m_libFault;
};

#endif // _BOARD_TEST_FAULT_H_
