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
        INITIALIZE         = (uint32)((uint32) 1U << 0U),
        MOVE_RELATIVE      = (uint32)((uint32) 1U << 1U),
        MOVE_ABSOLUTE      = (uint32)((uint32) 1U << 2U),
        MICRO_STEPS        = (uint32)((uint32) 3U << 3U),
        MICRO_STEPS_SET    = (uint32)((uint32) 1U << 6U),
        DIRECTION_POSITIVE = (uint32)((uint32) 1U << 7U),
        DIRECTION_NEGATIVE = (uint32)((uint32) 1U << 8U),
    };
    enum ControlShift {
        MICRO_STEPS_SHIFT = 3,
    };
    enum StatusShift {
        MOTOR_STATUS_SHIFT = 0,
    };
private:
    LibMotor m_libMotor;
    int m_direction;
    int m_relPosition;
    int m_absPosition;
    int m_microSteps;
    float m_speed;
    float m_acceleration;
    float m_deceleration;
    float m_holdCurrent;
};

#endif // _BOARD_TEST_MOTOR_H_
