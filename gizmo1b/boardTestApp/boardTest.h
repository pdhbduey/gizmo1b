#ifndef _BOARD_TEST_H_
#define _BOARD_TEST_H_

#include "sys_common.h"

class BoardTest
{
public:
    // Command
    enum Command {
        REG_READ  = 1, // register read
        REG_WRITE = 2, // register write
    };

    // Status
    enum Status {
        OKAY,       // Okay
        ERROR_CMD,  // Error unknown command
        ERROR_ADDR, // Error unknown register address
        ERROR_RO,   // Error read-only register
        ERROR_WO,   // Error write-only register
    };

    // Register memory map
    enum RegMemoryMap {
        // gizmo1b
        ADC_BASE    = 0x00000100,
        ADC_CONTROL = ADC_BASE,
        ADC_STATUS,
        ADC_RESULT,
        DAC_BASE  = 0x00000200,
        DAC_VALUE = DAC_BASE,
        DAC_STATUS,
        FAN_BASE  = 0x00000300,
        FAN_VALUE = FAN_BASE,
        FAULT_BASE  = 0x00000400,
        FAULT_RESET = FAULT_BASE,
        FAULT_STATE,
        DIO_BASE = 0x00000500,
        DIO_IN   = DIO_BASE,
        DIO_OUT,
        MOTOR_BASE    = 0x00000600,
        MOTOR_CONTROL = MOTOR_BASE,
        MOTOR_STATUS,
        MOTOR_RELATIVE_POSITION,
        MOTOR_ABSOLUTE_POSITION,
        MOTOR_SPEED,
        MOTOR_ACCELERATION,
        MOTOR_DECELERATION,
        MOTOR_HOLD_CURRENT,
        TEC_BASE    = 0x00000700,
        TEC_CONTROL = TEC_BASE,
        TEC_TIME_VALUE,
        TEC_CURRENT_VALUE,
        TEC_CURRENT_COUNT,
        TEC_CURRENT_LOOP_COUNT,
        TEC_ISENSE_VALUE,
        TEC_VSENSE_VALUE,
        TEC_LOOP_VALUE,
        TEC_COUNT_VALUE,
        THERMISTOR_BASE    = 0x00000800,
        THERMISTOR_CONTROL = THERMISTOR_BASE,
        THERMISTOR_STATUS,
        THERMISTOR_RESULT,
        LED_BASE    = 0x00000900,
        LED_CONTROL = LED_BASE,

    };
    BoardTest();
    virtual ~BoardTest();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
};

#endif // _BOARD_TEST_H_
