#include <LibAdcTempSensor2.h>
#include <mibspi.h>
#include <LibMibSpi5.h>
#include <LibMibSpi5GioPort.h>

LibAdcTempSensor2::LibAdcTempSensor2() :
    LibAD7699(new LibMibSpi5,
              LibMibSpi5::AD7699_16BIT_ADC,
              0, // no somi sw
              0,
              new LibWrapGioPort::Port(new LibMibSpi5GioPort, PIN_ENA), // 97:MIBSPI5NENA:T_ADC_CNV
              4.096,
              true)
{
    // Port direction
    mibspiREG5->PC1 |= (uint32)1U << PIN_CLK   // out
                    |  (uint32)1U << PIN_SIMO  // out
                    |  (uint32)0U << PIN_SOMI; // in
    // Port open drain
    mibspiREG5->PC6 |= (uint32)0U << PIN_CLK   // disable
                    |  (uint32)0U << PIN_SIMO  // disable
                    |  (uint32)0U << PIN_SOMI; // disable
    // Port pullup / pulldown selection
    mibspiREG5->PC8 |= (uint32)1U << PIN_CLK   // pull-up
                    |  (uint32)1U << PIN_SIMO  // pull-up
                    |  (uint32)1U << PIN_SOMI; // pull-up
    // Port pullup / pulldown enable
    mibspiREG5->PC7 |= (uint32)0U << PIN_CLK   // enable
                    |  (uint32)0U << PIN_SIMO  // enable
                    |  (uint32)0U << PIN_SOMI; // enable
    // Port pins to functional
    mibspiREG5->PC0 |= (uint32)1U << PIN_CLK   // functional
                    |  (uint32)1U << PIN_SIMO  // functional
                    |  (uint32)1U << PIN_SOMI; // functional
    mibspiPORT5->DIR |= (uint32)1U << PIN_ENA;
    initialize();
}
