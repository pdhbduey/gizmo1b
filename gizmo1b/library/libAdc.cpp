#include "libDelay.h"
#include "libWrapMibSpi5.h"
#include "libMutex.h"
#include "libAdc.h"

// Implements read/write after conversion
// This is what AD7699 data-sheet says about CNV, DATA, CLK and SDO:
// When CNV is brought low, SDO is driven from HighZ to MSB of the result.
// Falling clock edge clocks out result bits starting with MSB-1.
// MCU programmers manual says that clock rising edge should be used to sample SDO,
// which is the following clocking mode: Data is output one half-cycle before
// the first rising edge of SPICLK and on subsequent falling edges.
// Input data is latched on the rising edge of SPICLK (clock Mode with Polarity = 0 and Phase = 1)
// Driving CNV high starts the conversion, which lasts tCONV(max)=1.6usec @ 5V
// If at the end of conversions CNV is high, the busy indicator is disabled so we
// must ensure we don't pull CNV low until EOC.
// After the conversion, we can read the result. We need to allow time tACQ(min)=0.5usec @ 5V
// before we start a new conversion.

// Setup on MCU: 30Mhz clock, clock Mode with Polarity = 0 and Phase = 1

SemaphoreHandle_t LibAdc::s_mutex;
bool LibAdc::s_isInitialized;

LibAdc::LibAdc() :
    m_adcCnv(new LibWrapMibSpi5, PIN_SOMI) // 98:MIBSPI5SOMI[0]:ADC_CNV
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        // De-select ADC
        m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
        //gioSetBit(hetPORT1, PIN_HET_26, 1);
        // Allow enough time for internal reference to settle at turn-on
        LibDelay::waitForTimer(10000);
        // Perform 2 conversions to set CFG register to a known state
        float value;
        read(CHANNEL_0, value);
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
    uint16 cfg[2] = { 0, 0 };
    cfg[0] = (OVERWRITE_CFG              << CFG_SHIFT
           |  UNIPOLAR_REF_TO_GND        << IN_CH_CFG_SHIFT
           |  channel                    << IN_CH_SEL_SHIFT
           |  FULL_BW                    << FULL_BW_SEL_SHIFT
           |  EXT_REF                    << REF_SEL_SHIFT
           |  DISABLE_SEQ                << SEQ_EN_SHIFT
           |  READ_BACK_EN               << READ_BACK_SHIFT)
                                         << 2;
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, cfg);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    uint16 rbdata[2];
    m_libWrapMibSpi1.getData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, rbdata);
    LibDelay::waitForTimer(4); // wait for conversion & acquisition to complete
    return result;
}

int LibAdc::convert()
{
    int result = OKAY;
    uint16 cfg[2] = { 0, 0 };
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, cfg);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    uint16 rbdata[2];
    m_libWrapMibSpi1.getData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, rbdata);
    LibDelay::waitForTimer(4); // wait for conversion & acquisition to complete
    return result;
}

int LibAdc::get(uint16& data)
{
    int result = OKAY;
    uint16 cfg[2] = { 0, 0 };
    m_libWrapMibSpi1.setData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, cfg);
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, 1)) {
        result = ERROR_TIME_OUT;
    }
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    uint16 rbdata[2];
    m_libWrapMibSpi1.getData(LibWrapMibSpi1::AD7689ACPZ_8CH_16BIT_ADC, rbdata);
    data = rbdata[0];
    LibDelay::waitForTimer(4); // wait for conversion & acquisition to complete
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
