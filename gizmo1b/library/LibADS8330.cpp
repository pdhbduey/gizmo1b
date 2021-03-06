#include <LibADS8330.h>

// MCU setup: clock: 20Mhz,
//            mode: polarity = 1, phase = 1,
//            number of words: 1,
//            word lenght: 16bits

LibADS8330::LibADS8330(float vref = 5.0) :
    LibAdc(vref),
    m_somiSelect(0),
    m_libWrapMibSpi(0),
    m_mibSpiGroup(0)
{
    m_pinMap[SOMI_SW] = 0;
}

void LibADS8330::initialize()
{
    uint16 data;
    m_pinMap[CS]->m_libWrapGioPort->setPin(m_pinMap[CS]->m_pin, true);
    m_pinMap[CONVST]->m_libWrapGioPort->setPin(m_pinMap[CONVST]->m_pin, true);
    write(WRITE_CFG_REGISTER
        | SYSTEM_RESET, data);
    write(WRITE_CFG_REGISTER
        | INTERNAL_CCLK
        | MANUAL_TRIGGER
        | EOC_ACTIVE_LOW
        | PIN_EOC
        | EOC_OUT
        | DISABLE_POWER_DOWN
        | NORMAL_OPERATION, data);
    write(READ_CFG_REGISTER, data);
    write(SEL_AN_IN_CHANNEL_0, data);
}

int LibADS8330::read(int channel, uint32& value)
{
    int result = isChannelCorrect(channel);
    if (result != OKAY) {
        return result;
    }
    LibMibSpi::Lock lock(*m_libWrapMibSpi);
    if (m_pinMap[SOMI_SW]) {
        m_pinMap[SOMI_SW]->m_libWrapGioPort->setPin(m_pinMap[SOMI_SW]->m_pin, m_somiSelect);
    }
    // Select channel
    uint16 data;
    result = write(m_channelMap[channel], data);
    if (result != OKAY) {
        return result;
    }
    // Start conversion
    m_pinMap[CONVST]->m_libWrapGioPort->setPin(m_pinMap[CONVST]->m_pin, false);
    m_pinMap[CONVST]->m_libWrapGioPort->setPin(m_pinMap[CONVST]->m_pin, true);
    // Wait until EOC
    while (m_pinMap[EOC]->m_libWrapGioPort->getPin(m_pinMap[EOC]->m_pin) == false);
    // Read result
    result = write(READ_DATA, data);
    if (result != OKAY) {
        return result;
    }
    value = data;
    return result;
}

int LibADS8330::write(uint16 txBuffer, uint16& data)
{
    int result = OKAY;
    m_pinMap[CS]->m_libWrapGioPort->setPin(m_pinMap[CS]->m_pin, false);
    m_libWrapMibSpi->setData(m_mibSpiGroup, &txBuffer);
    m_libWrapMibSpi->transfer(m_mibSpiGroup);
    if (!m_libWrapMibSpi->waitForTransferComplete(m_mibSpiGroup, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_libWrapMibSpi->getData(m_mibSpiGroup, &data);
    m_pinMap[CS]->m_libWrapGioPort->setPin(m_pinMap[CS]->m_pin, true);
    return result;
}
