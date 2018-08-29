#include <libWrapGioPortA.h>
#include <LibCurrentDac1.h>
#include <LibMibSpi1.h>

LibCurrentDac1::LibCurrentDac1() :
    LibDAC8563(new LibMibSpi1, LibMibSpi1::DAC8563_16_BIT_DAC)
{
    LibWrapGioPort* libWrapGioPortA = new LibWrapGioPortA;
    m_dacCtrlMap[CLR]  = new LibWrapGioPort::Port(libWrapGioPortA, 5); // 14:GIOA[5]:DAC_CLR
    m_dacCtrlMap[SYNC] = new LibWrapGioPort::Port(libWrapGioPortA, 6); // 16:GIOA[6]:DAC_SYNC
    m_dacCtrlMap[LDAC] = new LibWrapGioPort::Port(libWrapGioPortA, 7); // 22:GIOA[7]:DAC_LDAC
    gioPORTA->DIR     |= (uint32)1U << 5
                      |  (uint32)1U << 6
                      |  (uint32)1U << 7;
    initialize();
}
