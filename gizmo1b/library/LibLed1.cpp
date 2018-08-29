#include <mibspi.h>
#include <LibLed1.h>
#include <LibMibSpi1GioPort.h>

LibLed1::LibLed1()
{
    m_ledMap[GREEN]   = new LibWrapGioPort::Port(new LibMibSpi1GioPort, PIN_ENA); //  96:MIBSPI1NENA:DEBUG_GREEN_LED
    m_ledMap[RED]     = new LibWrapGioPort::Port(new LibMibSpi1GioPort, PIN_CS0); // 105:MIBSPI1NCS[0]:ERROR_RED_LED
    mibspiPORT1->DIR |= (uint32)1U << PIN_ENA
                     |  (uint32)1U << PIN_CS0;
    set(GREEN, true);
    set(RED,   true);
}
