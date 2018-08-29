#include <mibspi.h>
#include <LibAdc1.h>
#include <LibMibSpi1.h>
#include <LibMibSpi3GioPort.h>
#include <LibMibSpi5GioPort.h>

LibAdc1::LibAdc1() :
    LibAD7699(new LibMibSpi1,
              LibMibSpi1::AD7699_16_BIT_ADC,
              new LibWrapGioPort::Port(new LibMibSpi3GioPort, PIN_ENA),  // 54:MIBSPI3NENA:MCU_SPI1_SOMI_SW
              LibMibSpi1::SPI_A,
              new LibWrapGioPort::Port(new LibMibSpi5GioPort, PIN_SOMI)) // 98:MIBSPI5SOMI[0]:ADC_CNV
{
    mibspiPORT3->DIR |= (uint32)1U << PIN_ENA;
    mibspiPORT5->DIR |= (uint32)1U << PIN_SOMI;
    initialize();
}
