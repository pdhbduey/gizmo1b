#include "libDac.h"
#include "libDelay.h"
#include "libMutex.h"
#include "libWrapGioPortA.h"

float LibDac::s_value;
SemaphoreHandle_t LibDac::s_mutex;
bool LibDac::s_isInitialized;

LibDac::LibDac()
{
    LibWrapGioPort* libWrapGioPortA = new LibWrapGioPortA;
    m_dacCtrlMap[CLR]  = new LibWrapGioPort::Port(libWrapGioPortA, 5); // 14:GIOA[5]:DAC_CLR
    m_dacCtrlMap[SYNC] = new LibWrapGioPort::Port(libWrapGioPortA, 6); // 16:GIOA[6]:DAC_SYNC
    m_dacCtrlMap[LDAC] = new LibWrapGioPort::Port(libWrapGioPortA, 7); // 22:GIOA[7]:DAC_LDAC
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();

        uint16 txBuffer[3];
        // Software reset
        txBuffer[0] = CMD_RESET << CMD_SHIFT;
        txBuffer[1] = 0;
        txBuffer[2] = POWER_ON_RESET;
        writeDac(txBuffer);
        // Disable internal reference and set gains to 1
        txBuffer[0] = CMD_ENABLE_INT_REF << CMD_SHIFT;
        txBuffer[1] = 0;
        txBuffer[2] = DISABLE_INT_REF_AND_RESET_DAC_GAINS_TO_1;
        writeDac(txBuffer);
        // Disable LDAC pins
        txBuffer[0] = CMD_SET_LDAC_PIN << CMD_SHIFT;
        txBuffer[1] = 0;
        txBuffer[2] = SET_LDAC_PIN_INACTIVE_DAC_B_INACTIVE_DAC_A;
        writeDac(txBuffer);
        // Power-down DAC-B
        txBuffer[0] = CMD_POWER_DAC << CMD_SHIFT;
        txBuffer[1] = 0;
        txBuffer[2] = POWER_DOWN_DAC_B_HI_Z;
        writeDac(txBuffer);
        s_value = 2.5;
        s_isInitialized = true;
    }
}

LibDac::~LibDac()
{
}

int LibDac::set(float value)
{
    LibMutex libMutex(s_mutex);
    if (value < 0.0 || value > 5.0) {
        return ERROR_SET_VALUE_OUT_OF_RANGE;
    }
    uint16 txBuffer[3];
    uint16 dacValue = value * (65535 / 5.0);
    // Write to DAC-A input register and update DAC-A
    txBuffer[0] = CMD_WR_ONE_REG_AND_UPDATE_ONE_DAC << CMD_SHIFT
                | ADDR_DAC_A << ADDR_SHIFT;
    txBuffer[1] = dacValue >> 8;
    txBuffer[2] = dacValue;
    int result = writeDac(txBuffer);
    if (result == OKAY) {
        s_value = value;
    }
    return result;
}

float LibDac::get()
{
    LibMutex libMutex(s_mutex);
    return s_value;
}

int LibDac::writeDac(uint16* txBuffer)
{
    int result = OKAY;
    m_libWrapMibSpi1.lock();
    m_dacCtrlMap[SYNC]->m_libWrapGioPort->setPin(m_dacCtrlMap[SYNC]->m_pin, false);
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::DAC8563SDGST_16_BIT_DAC, txBuffer);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::DAC8563SDGST_16_BIT_DAC);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::DAC8563SDGST_16_BIT_DAC, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_dacCtrlMap[SYNC]->m_libWrapGioPort->setPin(m_dacCtrlMap[SYNC]->m_pin, true);
    m_libWrapMibSpi1.unlock();
    return result;
}
