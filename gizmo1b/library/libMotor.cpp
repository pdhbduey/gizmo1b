#include <math.h>
#include "mibspi.h"
#include "libWrapMibSpi1.h"
#include "libWrapMibSpi3.h"
#include "libWrapMibSpi5.h"
#include "libDelay.h"
#include "libMutex.h"
#include "libMotor.h"

SemaphoreHandle_t LibMotor::s_mutex;
bool LibMotor::s_isInitialized;

LibMotor::LibMotor()
{
    LibWrapGioPort* libWrapMibSpi1 = &m_libWrapMibSpi1;
    LibWrapGioPort* libWrapMibSpi3 = new LibWrapMibSpi3;
    LibWrapGioPort* libWrapMibSpi5 = new LibWrapMibSpi5;
    m_pinMap[MOT_CS]        = new LibWrapGioPort::Port(libWrapMibSpi1, PIN_CS2); //  40:MIBSPI1NCS[2]:MOT_CS
    m_pinMap[MOT_FLAG]      = new LibWrapGioPort::Port(libWrapMibSpi1, PIN_CS1); // 130:MIBSPI1NCS[1]:MOT_FLAG
    m_pinMap[MOT_STCK]      = new LibWrapGioPort::Port(libWrapMibSpi3, PIN_CS3); //   3:MIBSPI3NCS[3]:MOT_STCK
    m_pinMap[MOT_SYNC]      = new LibWrapGioPort::Port(libWrapMibSpi3, PIN_CS2); //   4:MIBSPI3NCS[2]:MOT_SYNC
    m_pinMap[MOT_STBY_RST]  = new LibWrapGioPort::Port(libWrapMibSpi3, PIN_CS1); //  37:MIBSPI3NCS[1]:MOT_STBY/RST
    m_pinMap[MOT_SENSOR_IN] = new LibWrapGioPort::Port(libWrapMibSpi5, PIN_CLK); // 100:MIBSPI5CLK:MOT_SENSOR_IN
    m_addrToNoBytesMap[ABS_POS]    = 3;
    m_addrToNoBytesMap[EL_POS]     = 2;
    m_addrToNoBytesMap[MARK]       = 3;
    m_addrToNoBytesMap[SPEED]      = 3;
    m_addrToNoBytesMap[ACC]        = 2;
    m_addrToNoBytesMap[DEC]        = 2;
    m_addrToNoBytesMap[MAX_SPEED]  = 2;
    m_addrToNoBytesMap[MIN_SPEED]  = 2;
    m_addrToNoBytesMap[FS_SPD]     = 2;
    m_addrToNoBytesMap[KVAL_HOLD]  = 1;
    m_addrToNoBytesMap[KVAL_RUN]   = 1;
    m_addrToNoBytesMap[KVAL_ACC]   = 1;
    m_addrToNoBytesMap[KVAL_DEC]   = 1;
    m_addrToNoBytesMap[INT_SPEED]  = 2;
    m_addrToNoBytesMap[ST_SLP]     = 1;
    m_addrToNoBytesMap[FN_SLP_ACC] = 1;
    m_addrToNoBytesMap[FN_SLP_DEC] = 1;
    m_addrToNoBytesMap[K_THERM]    = 1;
    m_addrToNoBytesMap[ADC_OUT]    = 1;
    m_addrToNoBytesMap[OCD_TH]     = 1;
    m_addrToNoBytesMap[STALL_TH]   = 1;
    m_addrToNoBytesMap[STEP_MODE]  = 1;
    m_addrToNoBytesMap[ALARM_EN]   = 1;
    m_addrToNoBytesMap[STATUS]     = 2;
    m_addrToNoBytesMap[CONFIG]     = 2;
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
    reset();
    limp();
}

LibMotor::~LibMotor()
{
}

bool LibMotor::isValidReg(uint16 address)
{
    return m_addrToNoBytesMap.find(address) != m_addrToNoBytesMap.end();
}

int LibMotor::readReg(uint16 address, uint32& value)
{
    if (!isValidReg(address)) {
        return ERROR_INVALID_REG;
    }
    LibMutex libMutex(s_mutex);
    LibWrapMibSpi::Lock lock(m_libWrapMibSpi1);
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_B);
    uint16 data;
    int result = write(GET_PARAM | address, data);
    if (result == OKAY) {
        int nbytes = m_addrToNoBytesMap[address];
        uint32 v = 0;
        for (int i = 0; i < nbytes; i++) {
            result = write(NOP, data);
            if (result != OKAY) {
                break;
            }
            data &= 0xff;
            v |= data << (nbytes - 1 - i) * 8;
        }
        if (result == OKAY) {
            value = v;
        }
    }
    return result;
}

int LibMotor::writeReg(uint16 address, uint32 value)
{
    if (!isValidReg(address)) {
        return ERROR_INVALID_REG;
    }
    LibMutex libMutex(s_mutex);
    LibWrapMibSpi::Lock lock(m_libWrapMibSpi1);
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_B);
    uint16 data;
    int result = write(SET_PARAM | address, data);
    if (result == OKAY) {
        int nbytes = m_addrToNoBytesMap[address];
        for (int i = 0; i < nbytes; i++) {
            uint16 v = value >> (nbytes - 1 - i) * 8;
            v &= 0xff;
            result = write(v, data);
            if (result != OKAY) {
                break;
            }
        }
    }
    return result;
}

void LibMotor::reset()
{
    LibMutex libMutex(s_mutex);
    m_pinMap[MOT_STBY_RST]->m_libWrapGioPort->setPin(m_pinMap[MOT_STBY_RST]->m_pin, false);
    LibDelay::us(10);
    m_pinMap[MOT_STBY_RST]->m_libWrapGioPort->setPin(m_pinMap[MOT_STBY_RST]->m_pin, true);
}

int LibMotor::initialize()
{
    // 16MHz internal osc, 62.5kHz PWM, Overcurrent event shuts down bridges
    // MOT_SENSOR_IN/SW act as HardStop interrupt,
    uint16 value = 0;
    value |= 7 << F_PWM_DEC
          |  1 << OC_SD;
    return writeReg(CONFIG, value);
}

int LibMotor::write(uint16 command, uint16& data)
{
    int result = OKAY;
    m_pinMap[MOT_CS]->m_libWrapGioPort->setPin(m_pinMap[MOT_CS]->m_pin, false);
    LibDelay::us(1);
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::L6470HTR_STEPPER_MOTOR_DRIVER, &command);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::L6470HTR_STEPPER_MOTOR_DRIVER);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::L6470HTR_STEPPER_MOTOR_DRIVER, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_pinMap[MOT_CS]->m_libWrapGioPort->setPin(m_pinMap[MOT_CS]->m_pin, true);
    LibDelay::us(1);
    if (result == OKAY) {
        m_libWrapMibSpi1.getData(LibWrapMibSpi1::L6470HTR_STEPPER_MOTOR_DRIVER, &data);
    }
    return result;
}

int LibMotor::limp()
{
    LibMutex libMutex(s_mutex);
    LibWrapMibSpi::Lock lock(m_libWrapMibSpi1);
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_B);
    uint16 command = HARD_HIZ;
    uint16 data;
    int result = write(command, data);
    return result;
}

int LibMotor::energize()
{
    LibMutex libMutex(s_mutex);
    LibWrapMibSpi::Lock lock(m_libWrapMibSpi1);
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_B);
    uint16 command = HARD_STOP;
    uint16 data;
    int result = write(command, data);
    return result;
}

int LibMotor::setMicroSteps(int microSteps)
{
    if (microSteps <= STEP_MIN || microSteps >= STEP_MAX) {
        return ERROR_INVALID_MICROSTEPS;
    }
    return writeReg(STEP_MODE, microSteps);
}

int LibMotor::getMicroSteps(int& microSteps)
{
    uint32 value;
    int result = readReg(STEP_MODE, value);
    if (result == OKAY) {
        microSteps = value & STEP_SEL_MASK;
    }
    return result;
}

bool LibMotor::isValidDir(int direction)
{
    return direction ==  REVERSE || direction == FORWARD;
}

bool LibMotor::isValidPosition(sint32 position)
{
    return -2097152 <= position && position <= 2097151;
}

bool LibMotor::isValidSteps(uint32 steps)
{
    return steps <= 4194303;
}

int LibMotor::move(int command, uint32 steps)
{
    LibMutex libMutex(s_mutex);
    LibWrapMibSpi::Lock lock(m_libWrapMibSpi1);
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_B);
    uint16 data;
    int result = write(command, data);
    if (result == OKAY) {
        int nbytes = 3;
        for (int i = 0; i < nbytes; i++) {
            uint16 v = steps >> (nbytes - 1 - i) * 8;
            v &= 0xff;
            result = write(v, data);
            if (result != OKAY) {
                break;
            }
        }
    }
    return result;
}

int LibMotor::moveToPosition(sint32 position)
{
    if (!isValidPosition(position)) {
        return ERROR_STEPS_COUNT;
    }
    uint32 steps  = *reinterpret_cast<uint32*>(&position);
           steps &= 0x003fffff;
    return move(GOTO, steps);
}

int LibMotor::moveRelative(int direction, uint32 steps)
{
    if (!isValidDir(direction)) {
        return ERROR_INVALID_DIRECTION;
    }
    if (!isValidSteps(steps)) {
        return ERROR_STEPS_COUNT;
    }
    return move(MOVE | direction, steps);
}

int LibMotor::getStatus(uint32& status)
{
    uint32 value;
    int result = readReg(STATUS, value);
    if (result == OKAY) {
        status = 0;
        status |=   (value & STAT_HIZ                           ? HIZ         : 0);
        status |=   (value & STAT_BUSY                          ? 0           : BUSY);
        //status |=   (value & STAT_SW_STATE_CLOSED               ? SW_CLOSED   : SW_OPEN);
        status |=   (value & STAT_DIRECTION_FORWARD             ? DIR_FORWARD : DIR_REVERSE);
        status |=   (value & STAT_THERMAL_WARNING               ? 0           : THERMAL_WARNING);
        status |=   (value & STAT_THERMAL_SHUTDOWN              ? 0           : THERMAL_SHUTDOWN);
        status |=   (value & STAT_OVERCURRENT                   ? 0           : OVERCURRENT);
        status |=   (value & STAT_BRIDGE_A_STALL                ? 0           : BRIDGE_A_STALL);
        status |=   (value & STAT_BRIDGE_B_STALL                ? 0           : BRIDGE_B_STALL);
        status |=  ((value & MOTION_MASK) == STAT_MOTION_STATUS_STOPPED            ? STOPPED
               :    (value & MOTION_MASK) == STAT_MOTION_STATUS_ACCELERATING       ? ACCELERATING
               :    (value & MOTION_MASK) == STAT_MOTION_STATUS_DECELERATING       ? DECELERATING
               :    (value & MOTION_MASK) == STAT_MOTION_STATUS_RUN_AT_CONST_SPEED ? RUN_AT_CONST_SPEED : 0);
    }
    return result;
}

int LibMotor::getPosition(sint32& position)
{
    uint32 value;
    int result = readReg(ABS_POS, value);
    if (result == OKAY) {
        if (value & (1 << 21)) { // check if neg and expand to 32bits
            value |= 0xffc00000;
        }
        position = *reinterpret_cast<sint32*>(&value);
    }
    return result;
}

