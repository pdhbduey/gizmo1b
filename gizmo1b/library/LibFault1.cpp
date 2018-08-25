#include <LibFault1.h>
#include <LibMibSpi5GioPort.h>
#include <mibspi.h>
#include <libWrapGioPortA.h>

LibFault1::LibFault1()
{
    m_drvErrClr    = new LibWrapGioPort::Port(new LibMibSpi5GioPort, PIN_ENA); // 97:MIBSPI5NENA:DRV_ERR_CLR
    m_ntcMap[NTC1] = new LibWrapGioPort::Port(new LibMibSpi5GioPort, PIN_CS0); // 32:MIBSPI5NCS[0]:NTC_PRESENT1
    m_ntcMap[NTC2] = new LibWrapGioPort::Port(new LibWrapGioPortA, 2);         //  9:GIOA[2]:NTC_PRESENT2
    m_faultMap[TEC_OCD_POS] = 0; // not implemented
    m_faultMap[TEC_OCD_NEG] = 0;
    m_faultMap[OVERTEMP1]   = 0;
    m_faultMap[OVERTEMP2]   = 0;
}
