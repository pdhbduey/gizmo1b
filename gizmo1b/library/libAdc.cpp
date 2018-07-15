#include "libDelay.h"
#include "libWrapMibSpi5.h"
#include "libMutex.h"
#include "libAdc.h"

SemaphoreHandle_t LibAdc::s_mutex;
bool LibAdc::s_isInitialized;

LibAdc::LibAdc() :
    m_adcCnv(new LibWrapMibSpi5, PIN_SOMI) // 98:MIBSPI5SOMI[0]:ADC_CNV
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        // set ADC into the known state
        float dummy;
        read(CHANNEL_0, dummy);
        s_isInitialized = true;
    }
}

LibAdc::~LibAdc()
{
}

bool LibAdc::isChannelCorrect(int channel)
{
    int result = OKAY;
    switch (channel) {
    case CHANNEL_0:
    case CHANNEL_1:
    case CHANNEL_2:
    case CHANNEL_3:
    case CHANNEL_4:
    case CHANNEL_5:
        break;
    default:
        result = ERROR_INVALID_CHANNEL;
    }
    return result;
}

int LibAdc::set(int channel)
{
    int result = OKAY;
    uint16 cfg;
    cfg  = 0;
    cfg |= OVERWRITE_CFG       << CFG_SHIFT;
    cfg |= UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT;
    cfg |= channel             << IN_CH_SEL_SHIFT;
    cfg |= FULL_BW             << FULL_BW_SEL_SHIFT;
    cfg |= EXT_REF             << REF_SEL_SHIFT;
    cfg |= DISABLE_SEQ         << SEQ_EN_SHIFT;
    cfg |= READ_BACK_DISABLE   << READ_BACK_SHIFT;
    cfg  = cfg                 << 2;
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, &cfg);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    LibDelay::us(4); // wait for setup to complete
    return result;
}

int LibAdc::convert()
{
    int result = OKAY;
    uint16 cfg = 0;
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, &cfg);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    LibDelay::us(4); // wait for conversion to complete
    return result;
}

int LibAdc::get(uint16& data)
{
    int result = OKAY;
    uint16 cfg = 0;
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, &cfg);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    m_libWrapMibSpi1.getData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, &data);
    return result;
}

int LibAdc::read(int channel, uint32& value)
{
    int result = isChannelCorrect(channel);
    if (result != OKAY) {
        return result;
    }
    LibMutex libMutex(s_mutex);
    LibWrapMibSpi::Lock lock(m_libWrapMibSpi1);
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_A);
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

int LibAdc::read(int channel, float& value)
{
    uint32 data;
    int result = read(channel, data);
    if (result == OKAY) {
        value = data * (5.0 / 65535);
    }
    return result;
}
