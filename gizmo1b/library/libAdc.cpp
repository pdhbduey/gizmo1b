#include "libDelay.h"
#include "libWrapMibSpi5.h"
#include "libAdc.h"

LibAdc::LibAdc() :
    m_adcCnv(new LibWrapMibSpi5, PIN_SOMI) // 98:MIBSPI5SOMI[0]:ADC_CNV
{
    float dummy;
    read(CHANNEL_0, dummy);
}

LibAdc::~LibAdc()
{
}

int LibAdc::read(uint16* cfg, uint16* value)
{
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    LibDelay::us(3); // max conv time 2.2us
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    LibDelay::us(2); // min acq time 1.8us
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, cfg);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC);
    int result = OKAY;
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    else if (value) {
        m_libWrapMibSpi1.getData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, value);
    }
    return result;
}

int LibAdc::read(int channel, float& value)
{
    switch (channel) {
    case CHANNEL_0:
    case CHANNEL_1:
    case CHANNEL_2:
    case CHANNEL_3:
    case CHANNEL_4:
    case CHANNEL_5:
        break;
    default:
        return ERROR_INVALID_CHANNEL;
    }
    uint16 cfg =  OVERWRITE_CFG << CFG_SHIFT
               |  UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT
               |  channel << IN_CH_SEL_SHIFT
               |  FULL_BW << FULL_BW_SEL_SHIFT
               |  EXT_REF << REF_SEL_SHIFT
               |  DISABLE_SEQ << SEQ_EN_SHIFT
               |  READ_BACK_DISABLE << READ_BACK_SHIFT;
    cfg <<= 2;
    m_libWrapMibSpi1.lock();
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_A);
    int result = read(&cfg, (uint16*)NULL);
    if (result != OKAY) {
        m_libWrapMibSpi1.unlock();
        return result;
    }
    cfg = KEEP_CFG << CFG_SHIFT;
    cfg <<= 2;
    result = read(&cfg, (uint16*)NULL);
    if (result != OKAY) {
        m_libWrapMibSpi1.unlock();
        return result;
    }
    cfg = KEEP_CFG << CFG_SHIFT;
    cfg <<= 2;
    uint16 v;
    result = read(&cfg, &v);
    m_libWrapMibSpi1.unlock();
    if (result == OKAY) {
        value = v * (5.0 / 65535);
    }
    return result;
}

void LibAdc::test()
{
    LibAdc libAdc;
    float dummy;
    libAdc.read(CHANNEL_0, dummy);
    libAdc.read(CHANNEL_0, dummy);
    float vSense;
    int result = libAdc.read(CHANNEL_0, vSense);
    float iSense;
    result = libAdc.read(CHANNEL_1, iSense);
}
