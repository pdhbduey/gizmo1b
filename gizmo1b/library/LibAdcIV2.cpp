#include <LibAdcIV2.h>
#include <mibspi.h>
#include <LibMibSpi1.h>
#include <LibMibSpi1GioPort.h>
#include <LibMibSpi3GioPort.h>

LibAdcIV2::LibAdcIV2() :
    LibADS8330(4.096),
    m_isenseCorrection(0)
{
    m_libWrapMibSpi   = new LibMibSpi1;
    m_mibSpiGroup     = LibMibSpi1::ADS8330_16_BIT_ADC;
    m_pinMap[CONVST]  = new LibWrapGioPort::Port(new LibMibSpi1GioPort, PIN_CS2); // 40:MIBSPI1NCS[2]:IV_ADC_CNV
    m_pinMap[EOC]     = new LibWrapGioPort::Port(new LibMibSpi3GioPort, PIN_CS3); //  3:MIBSPI3NCS[3]:IV_ADC_EOC
    m_pinMap[CS]      = new LibWrapGioPort::Port(new LibMibSpi1GioPort, PIN_CS0); // 105:MIBSPI1NCS[0]:IV_ADC_CS
    mibspiPORT1->DIR |=   (uint32)1U << PIN_CS2
                     |    (uint32)1U << PIN_CS0;
    mibspiPORT3->DIR &= ~((uint32)1U << PIN_CS3);
    m_channelMap[ISENSE] = SEL_AN_IN_CHANNEL_0;
    m_channelMap[VSENSE] = SEL_AN_IN_CHANNEL_1;
    initialize();
}

void LibAdcIV2::initialize()
{
    LibADS8330::initialize();
    // Apply correction to Isense
    uint32 isense;
    if (LibADS8330::read(ISENSE, isense) == OKAY) {
        m_isenseCorrection = isense - (2.5 * 6.04/9.05) / m_vref * 65536;
    }
}

int LibAdcIV2::read(int channel, uint32& value)
{
    int result = LibADS8330::read(channel, value);
    if (result == OKAY) {
        value -= m_isenseCorrection;
    }
    return result;
}
