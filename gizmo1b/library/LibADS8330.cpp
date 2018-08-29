#include <LibADS8330.h>

// MCU setup: clock: 20Mhz,
//            mode: polarity = 0, phase = 0,
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
    m_pinMap[CS]->m_libWrapGioPort->setPin(m_pinMap[CS]->m_pin, true);
    m_pinMap[CONVST]->m_libWrapGioPort->setPin(m_pinMap[CONVST]->m_pin, true);
    uint16 data;
    write(RESET_CFG_REGISTER, data);
    uint16 cfg = WRITE_CFG_REGISTER
               | MANUAL_CHANNEL_SELECT
               | CCLK_INTERNAL_25MHz
               | MANUAL_TRIGGER_SOC
               | D8_DONT_CARE
               | EOC_ACTIVE_LOW
               | PIN_EOC_IS_EOC
               | PIN_EOC_IS_EOC_OUT
               | POWER_DOWN_DISABLED
               | TAG_BIT_DISABLED
               | NORMAL_OPERATION;
    write(cfg, data);
    write(READ_CFG_REGISTER, data);
    // assert( (cfg & ~(0b1111 << 12)) == (data & ~(0b1111 << 12)) );
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
    uint16 cfg = m_channelMap[channel];
    uint16 data;
    result = write(cfg, data);
    if (result != OKAY) {
        return result;
    }
    // Start conversion
    m_pinMap[CONVST]->m_libWrapGioPort->setPin(m_pinMap[CONVST]->m_pin, false);
    m_pinMap[CONVST]->m_libWrapGioPort->setPin(m_pinMap[CONVST]->m_pin, true);
    // Wait until EOC goes high indicating end of conversion
    while (m_pinMap[EOC]->m_libWrapGioPort->getPin(m_pinMap[EOC]->m_pin) == false);
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
    m_pinMap[CS]->m_libWrapGioPort->setPin(m_pinMap[CS]->m_pin, true);
    m_libWrapMibSpi->getData(m_mibSpiGroup, &data);
    return result;
}
