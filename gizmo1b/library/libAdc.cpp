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

// TIMING SPECIFICATIONS
// VDD = 4.5 V to 5.5 V, VIO = 1.8 V to VDD
// CNV Rising Edge to Data Available tCONV Max 2.2 탎
// ACQUISITION TIME tACQ                   Min 1.8 탎
// TIME BETWEEN CONVERSIONS tCYC           Min 4.0 탎
// DATA WRITE/READ DURING CONVERSION tDATA Max 1.2 탎 (SCK Min 13.3MHz)
// Pulse Width tCNVH                       Max 10 ns
// CNV MUST BE HIGH PRIOR TO THE END OF CONVERSION (EOC) TO AVOID THE BUSY INDICATOR
// CNV high transitions SDO to High-Z so CNV must be held low during the transfer
int LibAdc::readDataDuringConversion(uint16 cfg, uint16& data)
{
    int result = OKAY;
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true); // start conversion DATA(n-1), SDO to High-Z
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false); // MSB DATA(n-2) to SDO
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, &cfg);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC); // clock out CFG(n), clock in DATA(n-2)
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    if (result != OKAY) {
        return result;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true); // disable NO BUSY, SDO to High-Z
    m_libWrapMibSpi1.getData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, &data); // DATA(n-2) available
    LibDelay::us(4); // allow for conv of DATA(n-1) to complete, allow for acq of DATA(n) to complete
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    return result;
}

int LibAdc::read(int channel, float& value)
{
    LibMutex libMutex(s_mutex);
    int result = isChannelCorrect(channel);
    if (result != OKAY) {
        return result;
    }
    uint16 data;
    uint16 cfg = 0;
    cfg |= OVERWRITE_CFG << CFG_SHIFT;
    cfg |= UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT;
    cfg |= channel << IN_CH_SEL_SHIFT;
    cfg |= FULL_BW << FULL_BW_SEL_SHIFT;
    cfg |= EXT_REF << REF_SEL_SHIFT;
    cfg |= DISABLE_SEQ << SEQ_EN_SHIFT;
    cfg |= READ_BACK_DISABLE << READ_BACK_SHIFT;
    cfg <<= 2;
    m_libWrapMibSpi1.lock();
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_A);
    //
    //
    result = readDataDuringConversion(cfg, data); // conv DATA(n-1), clock out CFG(n), clock in DATA(n-2)
    if (result != OKAY) {
        m_libWrapMibSpi1.unlock();
        return result;
    }
    result = readDataDuringConversion(cfg, data); // conv DATA(n), clock out CFG(n+1), clock in DATA(n-1)
    if (result != OKAY) {
        m_libWrapMibSpi1.unlock();
        return result;
    }
    result = readDataDuringConversion(cfg, data); // conv DATA(n+1), clock out CFG(n+2), clock in DATA(n)
    if (result != OKAY) {
        m_libWrapMibSpi1.unlock();
        return result;
    }
    m_libWrapMibSpi1.unlock();
    value = data * (5.0 / 65535);
    return result;
}

void LibAdc::test()
{
    LibAdc libAdc;
    float vSense;
    int result = libAdc.read(CHANNEL_0, vSense);
    float iSense;
    result = libAdc.read(CHANNEL_1, iSense);
}
