#include <mibspi.h>
#include <gio.h>
#include <LibLed2.h>
#include <LibMibSpi1GioPort.h>
#include <LibMibSpi5GioPort.h>

LibLed2::LibLed2()
{
    m_ledMap[GREEN]   = new LibWrapGioPort::Port(new LibMibSpi1GioPort, PIN_ENA); // 96:MIBSPI1NENA:DEBUG_GREEN_LED
    m_ledMap[RED]     = new LibWrapGioPort::Port(new LibMibSpi5GioPort, PIN_CS0); // 32:MIBSPI5NCS[0]:ERROR_RED_LED
    mibspiPORT1->DIR |= (uint32)1U << PIN_ENA;
    mibspiPORT5->DIR |= (uint32)1U << PIN_CS0;
    set(GREEN, true);
    set(RED,   true);
}
