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
        FAN_BASE            = 0x00000300,
        FAN_PWM1_DUTY_CYCLE = FAN_BASE,
        FAN_PWM2_DUTY_CYCLE,
        FAN_PWM1_PERIOD_IN_US,
        FAN_PWM2_PERIOD_IN_US,
        FAN_STATUS,
        FAN_SENSOR1_RPM,
        FAN_SENSOR2_RPM,
        FAULT_BASE  = 0x00000400,
        FAULT_RESET = FAULT_BASE,
        FAULT_STATE,
        FAULT_NTC_PRESENT,
        DIO_BASE = 0x00000500,
        DIO_IN   = DIO_BASE,
        DIO_OUT,
        MOTOR_BASE    = 0x00000600,
        MOTOR_CONTROL = MOTOR_BASE,
        MOTOR_COMMUNICATION_STATUS,
        MOTOR_RELATIVE_POSITION,
        MOTOR_ABSOLUTE_POSITION,
        MOTOR_SPEED,
        MOTOR_ACCELERATION,
        MOTOR_DECELERATION,
        MOTOR_CURRENT_POSITION,
        MOTOR_STATUS,
        MOTOR_REGISTER_ADDRESS,
        MOTOR_REGISTER_VALUE,
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
        SNAPSHOT_BASE                     = 0x00000a00,
        SNAPSHOT_CONTROL                  = SNAPSHOT_BASE,
        SNAPSHOT_STATUS,
        SNAPSHOT_RESOLUTION,
        SNAPSHOT_NUMBER_OF_SAMPLES,
        SNAPSHOT_TEC_SAMPLES_RANGE,
        TRACE_CONTROL,
        TRACE_STATUS,
        TRACE_RESOLUTION,
        TRACE_FIRST_SAMPLE,
        TRACE_NUMBER_OF_SAMPLES,
        TRACE_NUMBER_OF_READ_SAMPLES,
        SNAPSHOT_SAMPLES_VSENSE           = 0x10000000,
        SNAPSHOT_SAMPLES_VSENSE_MAX       = SNAPSHOT_SAMPLES_VSENSE       + 999,
        SNAPSHOT_SAMPLES_ISENSE           = 0x20000000,
        SNAPSHOT_SAMPLES_ISENSE_MAX       = SNAPSHOT_SAMPLES_ISENSE       + 999,
        SNAPSHOT_SAMPLES_IREF             = 0x30000000,
        SNAPSHOT_SAMPLES_IREF_MAX         = SNAPSHOT_SAMPLES_IREF         + 999,
        SNAPSHOT_SAMPLES_TEMPERATURE1     = 0x40000000,
        SNAPSHOT_SAMPLES_TEMPERATURE1_MAX = SNAPSHOT_SAMPLES_TEMPERATURE1 + 999,
        SNAPSHOT_SAMPLES_TEMPERATURE2     = 0x50000000,
        SNAPSHOT_SAMPLES_TEMPERATURE2_MAX = SNAPSHOT_SAMPLES_TEMPERATURE2 + 999,
        SNAPSHOT_SAMPLES_TEMPERATURE3     = 0x60000000,
        SNAPSHOT_SAMPLES_TEMPERATURE3_MAX = SNAPSHOT_SAMPLES_TEMPERATURE3 + 999,
        SNAPSHOT_SAMPLES_TEMPERATURE4     = 0x70000000,
        SNAPSHOT_SAMPLES_TEMPERATURE4_MAX = SNAPSHOT_SAMPLES_TEMPERATURE4 + 999,
    };
    BoardTest();
    virtual ~BoardTest();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
};

#endif // _BOARD_TEST_H_
