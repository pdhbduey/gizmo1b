#include <LibAD5683.h>

// MCU setup: clock: 1MHz-20Mhz,
//            mode: polarity = 0, phase = 0,
//            number of words: 3,
//            word lenght: 8bits

LibAD5683::LibAD5683(LibMibSpi* libWrapMibSpi, uint32 mibSpiGroup, float refV) :
    m_libWrapMibSpi(*libWrapMibSpi),
    m_mibSpiGroup(mibSpiGroup),
    m_refV(refV)
{
}

void LibAD5683::initialize()
{
    LibMibSpi::Lock lock(m_libWrapMibSpi);
    m_dacSync->m_libWrapGioPort->setPin(m_dacSync->m_pin, true);
    uint16 txBuffer[3];
    uint32 ctrl;
    ctrl        = DAC_AD5683R_WRITE_CTRL_REGISTER << 20
                | DAC_AD5683R_RESET;
    txBuffer[0] = ctrl >> 16;
    txBuffer[1] = ctrl >> 8;
    txBuffer[2] = ctrl;
    writeDac(txBuffer);
    ctrl        = DAC_AD5683R_WRITE_CTRL_REGISTER << 20
                | DAC_AD5683R_NORMAL_MODE
                | DAC_AD5683R_ENABLE_REF
                | DAC_AD5683R_OUT_2xVREF
                | DAC_AD5683R_STANDALONE;
    txBuffer[0] = ctrl >> 16;
    txBuffer[1] = ctrl >> 8;
    txBuffer[2] = ctrl;
    writeDac(txBuffer);
    set((float) 2.5);
}

int LibAD5683::set(float value)
{
    LibMibSpi::Lock lock(m_libWrapMibSpi);
    if (value < 0.0 || value > m_refV) {
        return ERROR_SET_VALUE_OUT_OF_RANGE;
    }
    uint16 txBuffer[3];
    uint16 dacValue = value * (65535 / m_refV);
    uint32 ctrl = DAC_AD5683R_WRITE_DAC_AND_INPUT_REGISTER << 20
                | dacValue << 4;
    txBuffer[0] = ctrl >> 16;
    txBuffer[1] = ctrl >> 8;
    txBuffer[2] = ctrl;
    int result = writeDac(txBuffer);
    if (result == OKAY) {
        m_value = value;
    }
    return result;
}

int LibAD5683::set(uint32 value)
{
    LibMibSpi::Lock lock(m_libWrapMibSpi);
    if (value > 65535) {
        return ERROR_SET_VALUE_OUT_OF_RANGE;
    }
    uint16 txBuffer[3];
    uint16 dacValue = value;
    uint32 ctrl = DAC_AD5683R_WRITE_DAC_AND_INPUT_REGISTER << 20
                | dacValue << 4;
    txBuffer[0] = ctrl >> 16;
    txBuffer[1] = ctrl >> 8;
    txBuffer[2] = ctrl;
    int result = writeDac(txBuffer);
    if (result == OKAY) {
        m_value = (value * m_refV) / 65535;
    }
    return result;
}

int LibAD5683::writeDac(uint16* txBuffer)
{
    int result = OKAY;
    m_dacSync->m_libWrapGioPort->setPin(m_dacSync->m_pin, false);
    m_libWrapMibSpi.setData(m_mibSpiGroup, txBuffer);
    m_libWrapMibSpi.transfer(m_mibSpiGroup);
    if (!m_libWrapMibSpi.waitForTransferComplete(m_mibSpiGroup, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_dacSync->m_libWrapGioPort->setPin(m_dacSync->m_pin, true);
    return result;
}
