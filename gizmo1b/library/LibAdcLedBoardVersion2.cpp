#include <libDelay.h>
#include <libMutex.h>
#include <het.h>
#include <gio.h>
#include <libWrapHet1.h>
#include <LibAdcLedBoardVersion2.h>

// Implements read/write after conversion
// This is what AD7699 data-sheet says about CNV, DATA, CLK and SDO:
// When CNV is brought low, SDO is driven from HighZ to MSB of the result.
// Falling clock edge clocks out result bits starting with MSB-1.
// MCU programmers manual says that clock rising edge should be used to sample SDO,
// which is the following clocking mode: Data is output one half-cycle before
// the first rising edge of SPICLK and on subsequent falling edges.
// Input data is latched on the rising edge of SPICLK (clock Mode with Polarity = 0 and Phase = 1)
// Driving CNV high starts the conversion, which lasts tCONV(max)=1.6usec @ Vcc=5V
// If at the end of conversions CNV is high, the busy indicator is disabled so we
// must ensure we don't pull CNV low until EOC.
// After the conversion, we can read the result. We need to allow time tACQ(min)=0.5usec @ Vcc=5V
// before we start a new conversion.

SemaphoreHandle_t LibAdcLedBoardVersion2::s_mutex;
bool LibAdcLedBoardVersion2::s_isInitialized;

LibAdcLedBoardVersion2::LibAdcLedBoardVersion2() :
    m_adcCnv(new LibWrapHet1, PIN_HET_12), // 124:N2HET1[12]:DIG_OUT_A0:ADC_CNV
    m_refV(4.096)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
    // De-select ADC
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    // Allow enough time for internal reference to settle at turn-on
    LibDelay::pmuMicrosecDelay(10000);
    // Perform 2 conversions to set CFG register to a known state
    uint32 value;
    read(ADC_CHANNEL_0, value);
}

LibAdcLedBoardVersion2::~LibAdcLedBoardVersion2()
{
}

bool LibAdcLedBoardVersion2::isChannelCorrect(int channel)
{
    int result = OKAY;
    switch (channel) {
    case ADC_CHANNEL_0:
    case ADC_CHANNEL_1:
    case ADC_CHANNEL_2:
    case ADC_CHANNEL_3:
    case ADC_CHANNEL_4:
    case ADC_CHANNEL_5:
    case ADC_CHANNEL_6:
    case ADC_CHANNEL_7:
        break;
    default:
        result = ERROR_INVALID_CHANNEL;
    }
    return result;
}

int LibAdcLedBoardVersion2::set(int channel)
{
    int result = OKAY;
    uint16 cfg[2] = { 0, 0 };
    cfg[0] = (OVERWRITE_CFG              << CFG_SHIFT
           |  UNIPOLAR_REF_TO_GND        << IN_CH_CFG_SHIFT
           |  channel                    << IN_CH_SEL_SHIFT
           |  FULL_BW                    << FULL_BW_SEL_SHIFT
           |  INT_REF4_096_AND_TEMP_SENS << REF_SEL_SHIFT
           |  DISABLE_SEQ                << SEQ_EN_SHIFT
           |  READ_BACK_EN               << READ_BACK_SHIFT)
                                         << 2;
    //m_libMibSpi3.setData(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, cfg);
    //m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
//    m_libMibSpi3.transfer(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC);
//    if (!m_libMibSpi3.waitForTransferComplete(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, 1)) {
//        result = ERROR_TIME_OUT;
//    }
    //m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
//    LibDelay::waitForTimer(4); // wait for conversion & acquisition to complete
//    uint16 rbdata[2];
//    m_libMibSpi3.getData(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, rbdata);
    gioSetBit(hetPORT1, PIN_HET_12, 0);
    mibspiSetData(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, cfg);
    mibspiTransfer(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC);
    while(!(mibspiIsTransferComplete(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC)));
    gioSetBit(hetPORT1, PIN_HET_12, 1);
    LibDelay::pmuMicrosecDelay(4);
    uint16 rbdata[2];
    mibspiGetData(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, rbdata);
    return result;
}

int LibAdcLedBoardVersion2::convert()
{
    int result = OKAY;
    uint16 cfg[2] = { 0, 0 };
//    m_libMibSpi3.setData(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, cfg);
//    //m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
//    m_libMibSpi3.transfer(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC);
//    if (!m_libMibSpi3.waitForTransferComplete(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, 1)) {
//        result = ERROR_TIME_OUT;
//    }
//    //m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
//    LibDelay::waitForTimer(4); // wait for conversion & acquisition to complete
//    uint16 rbdata[2];
//    m_libMibSpi3.getData(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, rbdata);
    gioSetBit(hetPORT1, PIN_HET_12, 0);
    mibspiSetData(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, cfg);
    mibspiTransfer(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC);
    while(!(mibspiIsTransferComplete(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC)));
    gioSetBit(hetPORT1, PIN_HET_12, 1);
    LibDelay::pmuMicrosecDelay(4);
    uint16 rbdata[2];
    mibspiGetData(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, rbdata);
    return result;
}

int LibAdcLedBoardVersion2::get(uint16& data)
{
    int result = OKAY;
    uint16 cfg[2] = { 0, 0 };
//    m_libMibSpi3.setData(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, cfg);
//    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
//    m_libMibSpi3.transfer(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC);
//    if (!m_libMibSpi3.waitForTransferComplete(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, 1)) {
//        result = ERROR_TIME_OUT;
//    }
//    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
//    LibDelay::waitForTimer(4); // wait for conversion & acquisition to complete
//    uint16 rbdata[2];
//    m_libMibSpi3.getData(LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, rbdata);
//    data = rbdata[0];
    gioSetBit(hetPORT1, PIN_HET_12, 0);
    mibspiSetData(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, cfg);
    mibspiTransfer(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC);
    while(!(mibspiIsTransferComplete(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC)));
    gioSetBit(hetPORT1, PIN_HET_12, 1);
    LibDelay::pmuMicrosecDelay(4);
    uint16 rbdata[2];
    mibspiGetData(mibspiREG3, LibMibSpi3::AD7699BCPZ_8CH_16BIT_ADC, rbdata);
    data = rbdata[0];
    return result;
}

int LibAdcLedBoardVersion2::read(int channel, uint32& value)
{
    int result = isChannelCorrect(channel);
    if (result != OKAY) {
        return result;
    }
    LibMutex libMutex(s_mutex);
    LibWrapMibSpi::Lock lock(m_libMibSpi3);
    m_libMibSpi3.somiSelect(LibMibSpi3::SPI_A);
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

int LibAdcLedBoardVersion2::read(int channel, float& value)
{
    uint32 data;
    int result = read(channel, data);
    if (result == OKAY) {
        value = data * (m_refV / 65535);
    }
    return result;
}

float LibAdcLedBoardVersion2::getVref()
{
    return m_refV;
}
