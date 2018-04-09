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
        TEC_IREF_VALUE,
        TEC_ISENSE_VALUE,
        TEC_VSENSE_VALUE,
        TEC_STATUS,
        TEC_WAVEFORM_TYPE,
        TEC_WAVEFORM_PERIOD,
        TEC_UNUSED,
        TEC_PROPORTIONAL_GAIN,
        TEC_INTEGRAL_GAIN,
        TEC_DERIVATIVE_GAIN,
        TEC_WAVEFORM_SAMPLE_INDEX,
        TEC_WAVEFORM_SAMPLE_TIME,
        TEC_WAVEFORM_SAMPLE_IREF,
        TEC_WAVEFORM_CYCLES,
        THERMISTOR_BASE   = 0x00000800,
        THERMISTOR_STATUS = THERMISTOR_BASE,
        THERMISTOR_RESULT_AIN_A,
        THERMISTOR_RESULT_AIN_B,
        THERMISTOR_RESULT_AIN_C,
        THERMISTOR_RESULT_AIN_D,
        LED_BASE    = 0x00000900,
        LED_CONTROL = LED_BASE,

    };
    BoardTest();
    virtual ~BoardTest();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
};

#endif // _BOARD_TEST_H_
