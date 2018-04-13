#ifndef _BOARD_TEST_MOTOR_H_
#define _BOARD_TEST_MOTOR_H_

#include "boardTest.h"
#include "libMotor.h"

class BoardTestMotor: public BoardTest
{
public:
    BoardTestMotor();
    virtual ~BoardTestMotor();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
    enum Control {
        INITIALIZE         = (uint32)((uint32) 1U <<  0U),
        MOVE_RELATIVE      = (uint32)((uint32) 1U <<  1U),
        MOVE_ABSOLUTE      = (uint32)((uint32) 2U <<  1U),
        RUN_AT_CONST_SPEED = (uint32)((uint32) 3U <<  1U),
        LIMP               = (uint32)((uint32) 4U <<  1U),
        ENERGIZE           = (uint32)((uint32) 5U <<  1U),
        MOVE_TO_HOME_POS   = (uint32)((uint32) 6U <<  1U),
        SEARCH_HOME_POS    = (uint32)((uint32) 7U <<  1U),
        STEP_FULL          = (uint32)((uint32) 1U <<  5U),
        STEP_HALF          = (uint32)((uint32) 2U <<  5U),
        STEP_MICRO_1_4TH   = (uint32)((uint32) 3U <<  5U),
        STEP_MICRO_1_8TH   = (uint32)((uint32) 4U <<  5U),
        STEP_MICRO_1_16TH  = (uint32)((uint32) 5U <<  5U),
        STEP_MICRO_1_32TH  = (uint32)((uint32) 6U <<  5U),
        STEP_MICRO_1_64TH  = (uint32)((uint32) 7U <<  5U),
        STEP_MICRO_1_128TH = (uint32)((uint32) 8U <<  5U),
        DIRECTION_REVERSE  = (uint32)((uint32) 1U <<  9U),
        DIRECTION_FORWARD  = (uint32)((uint32) 1U << 10U),
        REGISTER_READ      = (uint32)((uint32) 1U << 11U),
        REGISTER_WRITE     = (uint32)((uint32) 1U << 12U),
        RESET              = (uint32)((uint32) 1U << 13U),
    };
    enum Masks {
        MOVE_MASK = 0xf << 1,
        STEP_MASK = 0xf << 5,
    };
private:
    LibMotor m_libMotor;
    int m_status;
    uint16 m_registerAddress;
    uint32 m_registerValue;
    sint32 m_absPosition;
    uint32 m_relPosition;
};

#endif // _BOARD_TEST_MOTOR_H_
