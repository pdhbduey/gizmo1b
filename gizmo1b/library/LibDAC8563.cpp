#include <libWrapGioPortA.h>
#include <LibDAC8563.h>
#include <LibDelay.h>

LibDAC8563::LibDAC8563(LibMibSpi* libWrapMibSpi, uint32 mibSpiGroup, float refV) :
    m_libWrapMibSpi(*libWrapMibSpi),
    m_mibSpiGroup(mibSpiGroup),
    m_refV(refV)
{
}

void LibDAC8563::initialize()
{
    LibMibSpi::Lock lock(m_libWrapMibSpi);
    m_dacCtrlMap[CLR]->m_libWrapGioPort->setPin(m_dacCtrlMap[CLR]->m_pin, true);
    m_dacCtrlMap[SYNC]->m_libWrapGioPort->setPin(m_dacCtrlMap[SYNC]->m_pin, true);
    m_dacCtrlMap[LDAC]->m_libWrapGioPort->setPin(m_dacCtrlMap[LDAC]->m_pin, true);
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
    m_value = 2.5;
}

int LibDAC8563::set(float value)
{
    LibMibSpi::Lock lock(m_libWrapMibSpi);
    if (value < 0.0 || value > m_refV) {
        return ERROR_SET_VALUE_OUT_OF_RANGE;
    }
    uint16 txBuffer[3];
    uint16 dacValue = value * (65535 / m_refV);
    // Write to DAC-A input register and update DAC-A
    txBuffer[0] = CMD_WR_ONE_REG_AND_UPDATE_ONE_DAC << CMD_SHIFT
                | ADDR_DAC_A << ADDR_SHIFT;
    txBuffer[1] = dacValue >> 8;
    txBuffer[2] = dacValue;
    int result = writeDac(txBuffer);
    if (result == OKAY) {
        m_value = value;
    }
    return result;
}

int LibDAC8563::set(uint32 value)
{
    LibMibSpi::Lock lock(m_libWrapMibSpi);
    if (value > 65535) {
        return ERROR_SET_VALUE_OUT_OF_RANGE;
    }
    uint16 txBuffer[3];
    uint16 dacValue = value;
    // Write to DAC-A input register and update DAC-A
    txBuffer[0] = CMD_WR_ONE_REG_AND_UPDATE_ONE_DAC << CMD_SHIFT
                | ADDR_DAC_A << ADDR_SHIFT;
    txBuffer[1] = dacValue >> 8;
    txBuffer[2] = dacValue;
    int result = writeDac(txBuffer);
    if (result == OKAY) {
        m_value = (value * m_refV) / 65535;
    }
    return result;
}

int LibDAC8563::writeDac(uint16* txBuffer)
{
    int result = OKAY;
    m_dacCtrlMap[SYNC]->m_libWrapGioPort->setPin(m_dacCtrlMap[SYNC]->m_pin, false);
    m_libWrapMibSpi.setData(m_mibSpiGroup, txBuffer);
    m_libWrapMibSpi.transfer(m_mibSpiGroup);
    if (!m_libWrapMibSpi.waitForTransferComplete(m_mibSpiGroup, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_dacCtrlMap[SYNC]->m_libWrapGioPort->setPin(m_dacCtrlMap[SYNC]->m_pin, true);
    return result;
}
