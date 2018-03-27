#include "libDelay.h"
#include "libWrapMibSpi5.h"
#include "libAdc.h"

LibAdc::LibAdc() :
    m_adcCnv(new LibWrapMibSpi5, PIN_SOMI) // 98:MIBSPI5SOMI[0]:ADC_CNV
{
    // Initialize ADC
}

LibAdc::~LibAdc()
{
}

int LibAdc::setChannel(int channel)
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
    m_libWrapMibSpi1.lock();
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_A);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    LibDelay::us(10);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    uint16 cfg = 0;
    cfg |= OVERWRITE_CFG << CFG_SHIFT
        |  UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT
        |  channel << IN_CH_SEL_SHIFT
        |  FULL_BW << FULL_BW_SEL_SHIFT
        |  EXT_REF << REF_SEL_SHIFT
        |  DISABLE_SEQ << SEQ_EN_SHIFT
        |  READ_BACK_DISABLE << READ_BACK_SHIFT;
    int result = OKAY;
    m_libWrapMibSpi1.lock();
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, &cfg);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC);
    if (m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_libWrapMibSpi1.unlock();
    return result;
}

int LibAdc::read(float& value)
{
    m_libWrapMibSpi1.lock();
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_A);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    LibDelay::us(10);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    uint16 cfg = 0;
    cfg = KEEP_CFG << CFG_SHIFT;
    int result = OKAY;
    m_libWrapMibSpi1.lock();
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, &cfg);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC);
    if (m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    else {
        uint16 v;
        m_libWrapMibSpi1.getData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, &v);
        value = v * (5.0 / 65536);
    }
    m_libWrapMibSpi1.unlock();
    return result;
}
