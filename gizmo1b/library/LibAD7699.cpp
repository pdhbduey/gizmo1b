#include <LibAD7699.h>
#include <LibDelay.h>

// MCU setup: clock: 1MHz-20Mhz,
//            mode: polarity = 0, phase = 1,
//            number of words: 1 (READ_BACK_DISABLE) or 2(READ_BACK_EN),
//            word lenght: 16bits

LibAD7699::LibAD7699(LibMibSpi* libWrapMibSpi,
                     uint32 mibSpiGroup,
                     LibWrapGioPort::Port* somiSw,
                     uint32 somiSelect,
                     LibWrapGioPort::Port* adcCnv,
                     float vref,
                     bool isIntVref) :
    m_libWrapMibSpi(*libWrapMibSpi),
    m_mibSpiGroup(mibSpiGroup),
    m_somiSw(somiSw),
    m_somiSelect(somiSelect),
    m_adcCnv(*adcCnv),
    LibAdc(vref),
    m_isIntVref(isIntVref)
{
}

void LibAD7699::initialize()
{
    // De-select ADC
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    // Allow enough time for internal reference to settle at turn-on
    LibDelay::pmuMicrosecDelay(10000);
    // Perform 2 conversions to set CFG register to a known state
    uint32 value;
    read(CHANNEL_0, value);
}

int LibAD7699::set(int channel)
{
    int result = OKAY;
    uint16 cfg[2] = { 0, 0 };
    cfg[0] = (OVERWRITE_CFG                                       << CFG_SHIFT
           |  UNIPOLAR_REF_TO_GND                                 << IN_CH_CFG_SHIFT
           |  channel                                             << IN_CH_SEL_SHIFT
           |  FULL_BW                                             << FULL_BW_SEL_SHIFT
           | (m_isIntVref ? INT_REF4_096_AND_TEMP_SENS : EXT_REF) << REF_SEL_SHIFT
           |  DISABLE_SEQ                                         << SEQ_EN_SHIFT
           |  READ_BACK_EN                                        << READ_BACK_SHIFT)
                                                                  << 2;
    m_libWrapMibSpi.setData(m_mibSpiGroup, cfg);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    m_libWrapMibSpi.transfer(m_mibSpiGroup);
    if (!m_libWrapMibSpi.waitForTransferComplete(m_mibSpiGroup, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    uint16 rbdata[2];
    m_libWrapMibSpi.getData(m_mibSpiGroup, rbdata);
    LibDelay::pmuMicrosecDelay(4); // wait for conversion & acquisition to complete
    return result;
}

int LibAD7699::convert()
{
    int result = OKAY;
    uint16 cfg[2] = { 0, 0 };
    m_libWrapMibSpi.setData(m_mibSpiGroup, cfg);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    m_libWrapMibSpi.transfer(m_mibSpiGroup);
    if (!m_libWrapMibSpi.waitForTransferComplete(m_mibSpiGroup, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    uint16 rbdata[2];
    m_libWrapMibSpi.getData(m_mibSpiGroup, rbdata);
    LibDelay::pmuMicrosecDelay(4); // wait for conversion & acquisition to complete
    return result;
}

int LibAD7699::get(uint16& data)
{
    int result = OKAY;
    uint16 cfg[2] = { 0, 0 };
    m_libWrapMibSpi.setData(m_mibSpiGroup, cfg);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    m_libWrapMibSpi.transfer(m_mibSpiGroup);
    if (!m_libWrapMibSpi.waitForTransferComplete(m_mibSpiGroup, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    uint16 rbdata[2];
    m_libWrapMibSpi.getData(m_mibSpiGroup, rbdata);
    data = rbdata[0];
    LibDelay::pmuMicrosecDelay(4); // wait for conversion & acquisition to complete
    return result;
}

int LibAD7699::read(int channel, uint32& value)
{
    int result = isChannelCorrect(channel);
    if (result != OKAY) {
        return result;
    }
    LibMibSpi::Lock lock(m_libWrapMibSpi);
    if (m_somiSw) {
        m_somiSw->m_libWrapGioPort->setPin(m_somiSw->m_pin, m_somiSelect);
    }
    result = set(channel);
    if (result != OKAY) {
        return result;
    }
    result = convert();
    if (result != OKAY) {
        return result;
    }
    uint16 data;
    result = get(data);
    if (result != OKAY) {
        return result;
    }
    value = data;
    return result;
}
