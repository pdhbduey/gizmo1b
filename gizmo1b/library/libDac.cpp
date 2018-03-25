#include "libWrapGioPortA.h"
#include "libDac.h"
#include "libDelay.h"

LibDac::LibDac()
{
    LibWrapGioPort* libWrapGioPortA = new LibWrapGioPortA;
    m_dacCtrlMap[CLR]  = new LibWrapGioPort::Port(libWrapGioPortA, 5); // 14:GIOA[5]:DAC_CLR
    m_dacCtrlMap[SYNC] = new LibWrapGioPort::Port(libWrapGioPortA, 6); // 16:GIOA[6]:DAC_SYNC
    m_dacCtrlMap[LDAC] = new LibWrapGioPort::Port(libWrapGioPortA, 7); // 22:GIOA[7]:DAC_LDAC
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
    // Clear output
    txBuffer[0] = CMD_WR_ONE_REG_AND_UPDATE_ONE_DAC << CMD_SHIFT
                | ADDR_DAC_A << ADDR_SHIFT;
    txBuffer[1] = 0;
    txBuffer[2] = 0;
    int result = writeDac(txBuffer);
}

LibDac::~LibDac()
{
}

int LibDac::set(float value)
{
    if (value < 0.0 || value > 5.0) {
        return ERROR_SET_VALUE_OUT_OF_RANGE;
    }
    uint16 txBuffer[3];
    uint16 adcValue = value * (65535 / 5.0);
    // Write to DAC-A input register and update DAC-A
    txBuffer[0] = CMD_WR_ONE_REG_AND_UPDATE_ONE_DAC << CMD_SHIFT
                | ADDR_DAC_A << ADDR_SHIFT;
    txBuffer[1] = adcValue >> 8;
    txBuffer[2] = adcValue;
    int result = writeDac(txBuffer);
    return result;
}

int LibDac::writeDac(uint16* txBuffer)
{
    int result = OKAY;
    m_libWrapMibSpi1.lock();
    m_dacCtrlMap[SYNC]->m_libWrapGioPort->setPin(m_dacCtrlMap[SYNC]->m_pin, false);
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::DAC8563SDGST_16_BIT_DAC, txBuffer);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::DAC8563SDGST_16_BIT_DAC);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi::LOOP_BACK_TEST, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_dacCtrlMap[SYNC]->m_libWrapGioPort->setPin(m_dacCtrlMap[SYNC]->m_pin, true);
    m_libWrapMibSpi1.unlock();
    return result;
}

void LibDac::test()
{
    LibDac libDac;
    while (true) {
        for (int i = 0; i < 100; i++) {
            libDac.set(5.0 / 100 * i);
            LibDelay::us(1);
        }
        for (int i = 0; i < 100; i++) {
            int j = 100 - i;
            libDac.set(5.0 / 100 * j);
            LibDelay::us(1);
        }
    }
}
