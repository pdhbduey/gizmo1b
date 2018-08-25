#ifndef _LIB_MOTOR_H_
#define _LIB_MOTOR_H_

#include <map>
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "libWrapGioPort.h"
#include "LibMibSpi1.h"
#include <LibMibSpi1GioPort.h>

class LibMotor
{
public:
    enum Direction {
        REVERSE,
        FORWARD,
    };
    enum Microsteps {
        STEP_FULL,
        STEP_HALF,
        STEP_MICRO_1_4TH,
        STEP_MICRO_1_8TH,
        STEP_MICRO_1_16TH,
        STEP_MICRO_1_32TH,
        STEP_MICRO_1_64TH,
        STEP_MICRO_1_128TH,

        STEP_MAX,
        STEP_MIN = STEP_FULL - 1,
    };
    enum CommunicationStatus {
        OKAY,
        ERROR_INVALID_DIRECTION,
        ERROR_STEPS_COUNT,
        ERROR_INVALID_MICROSTEPS,
        ERROR_TIME_OUT,
        ERROR_INVALID_REG,
    };
    enum Status {
        HIZ                           = 1 <<  0,
        BUSY                          = 1 <<  1,
        SW_CLOSED                     = 1 <<  2,
        SW_OPEN                       = 1 <<  3,
        DIR_FORWARD                   = 1 <<  4,
        DIR_REVERSE                   = 1 <<  5,
        STOPPED                       = 1 <<  6,
        ACCELERATING                  = 1 <<  7,
        DECELERATING                  = 1 <<  8,
        RUN_AT_CONST_SPEED            = 1 <<  9,
        THERMAL_WARNING               = 1 << 11,
        THERMAL_SHUTDOWN              = 1 << 12,
        OVERCURRENT                   = 1 << 13,
        BRIDGE_A_STALL                = 1 << 14,
        BRIDGE_B_STALL                = 1 << 15,
    };
    enum RegisterAddress {
        ABS_POS    =    1, // Current position 22 000000 0 R, WS
        EL_POS     =    2, // Electrical position 9 000 0 R, WS
        MARK       =    3, // Mark position 22 000000 0 R, WR
        SPEED      =    4, // Current speed 20 00000 0 step/tick (0 step/s) R
        ACC        =    5, // Acceleration 12 08A 125.5e-12 step/tick2 (2008 step/s2) R, WS
        DEC        =    6, // Deceleration 12 08A 125.5e-12 step/tick2 (2008 step/s2) R, WS
        MAX_SPEED  =    7, // Maximum speed 10 041 248e-6 step/tick (991.8 step/s) R, WR
        MIN_SPEED  =    8, // Minimum speed 13 000 0 step/tick (0 step/s) R, WS
        FS_SPD     = 0x15, // Full-step speed 10 027 150.7e-6 step/tick (602.7 step/s) R, WR
        KVAL_HOLD  =    9, // Holding KVAL 8 29 0.16·VS R, WR
        KVAL_RUN   = 0x0a, // Constant speed KVAL 8 29 0.16·VS R, WR
        KVAL_ACC   = 0x0b, // Acceleration starting KVAL 8 29 0.16·VS R, WR
        KVAL_DEC   = 0x0c, // Deceleration starting KVAL 8 29 0.16·VS R, WR
        INT_SPEED  = 0x0d, // Intersect speed 14 0408 15.4e-6 step/tick (61.5 step/s) R, WH
        ST_SLP     = 0x0e, // Start slope 8 19 0.038% s/step R, WH
        FN_SLP_ACC = 0x0f, // Acceleration final slope 8 29 0.063% s/step R, WH
        FN_SLP_DEC = 0x10, // Deceleration final slope 8 29 0.063% s/step R, WH
        K_THERM    = 0x11, // Thermal compensation factor 4 0 1.0 R, WR
        ADC_OUT    = 0x12, // ADC output 5 XX(2) R
        OCD_TH     = 0x13, // OCD threshold 4 8 3.38A R, WR
        STALL_TH   = 0x14, // STALL threshold 7 40 2.03A R, WR
        STEP_MODE  = 0x16, // Step mode 8 7 128 microsteps R, WH
        ALARM_EN   = 0x17, // Alarm enable 8 FF All alarms enabled R, WS
        CONFIG     = 0x18, // IC configuration 16 2E88 R, WH
        STATUS     = 0x19, // 16 XXXX(2) High impedance state, UVLO/Reset flag set. R
    };
public:
    LibMotor();
    void reset();
    int limp();
    int energize();
    int initialize();
    int setMicroSteps(int microSteps);
    int getMicroSteps(int& microSteps);
    int readReg(uint16 address, uint32& value);
    int writeReg(uint16 address, uint32 value);
    int moveToPosition(sint32 position);           // -2097152..2097151
    int moveRelative(int direction, uint32 steps); // 0..4194303
    int getStatus(uint32& status);
    int getPosition(sint32& position);
private:
    enum Pins {
        MOT_CS,
        MOT_FLAG,
        MOT_STCK,
        MOT_SYNC,
        MOT_STBY_RST,
        MOT_SENSOR_IN,
        MOT_SOMI_SW,
    };
    enum Commands {
        NOP       = 0,
        SET_PARAM = 0,
        GET_PARAM = 0x20,
        MOVE      = 0x40,
        GOTO      = 0x60,
        HARD_STOP = 0xb8,
        HARD_HIZ  = 0xa8,
    };
    enum ConfigRegShifts {
        F_PWM_INT = 13,
        F_PWM_DEC = 10,
        POW_SR    =  8,
        OC_SD     =  7,
        EN_VSCOMP =  5,
        SW_MODE   =  4,
        EXT_CLK   =  3,
        OSC_SEL   =  0,
    };
    enum StepModeMask {
        STEP_SEL_MASK = 7,
    };
    enum MotorStatus {
        STAT_HIZ                              = 1 <<  0,
        STAT_BUSY                             = 1 <<  1,
        STAT_SW_STATE_CLOSED                  = 1 <<  2,
        STAT_DIRECTION_FORWARD                = 1 <<  4,
        STAT_MOTION_STATUS_STOPPED            = 0 <<  5,
        STAT_MOTION_STATUS_ACCELERATING       = 1 <<  5,
        STAT_MOTION_STATUS_DECELERATING       = 2 <<  5,
        STAT_MOTION_STATUS_RUN_AT_CONST_SPEED = 3 <<  5,
        STAT_THERMAL_WARNING                  = 1 << 10,
        STAT_THERMAL_SHUTDOWN                 = 1 << 11,
        STAT_OVERCURRENT                      = 1 << 12,
        STAT_BRIDGE_A_STALL                   = 1 << 13,
        STAT_BRIDGE_B_STALL                   = 1 << 14,
    };
    enum MotorStausMasks {
        MOTION_MASK = 0x3 << 5,
    };
private:
    int write(uint16 data, uint16& result);
    bool isValidReg(uint16 address);
    bool isValidDir(int direction);
    bool isValidPosition(sint32 position);
    bool isValidSteps(uint32 steps);
    int move(int command, uint32 steps);
private:
	LibMibSpi1 m_libWrapMibSpi1;
    std::map<int, LibWrapGioPort::Port*> m_pinMap;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    std::map<int, int> m_addrToNoBytesMap;
};

#endif // _LIB_MOTOR_H_
