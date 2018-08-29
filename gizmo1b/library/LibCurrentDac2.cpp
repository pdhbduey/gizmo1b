#include <LibCurrentDac2.h>
#include <gio.h>
#include <LibMibSpi1.h>
#include <LibMibSpi1GioPort.h>

LibCurrentDac2::LibCurrentDac2() :
    LibAD5683(new LibMibSpi1, LibMibSpi1::AD5683_16_BIT_DAC)
{
    m_dacSync = new LibWrapGioPort::Port(new LibMibSpi1GioPort, PIN_CS1); // 130:MIBSPI1NCS[1]:I_DAC_SYNC
    mibspiPORT1->DIR |= (uint32)1U << PIN_CS1;
    initialize();
}
