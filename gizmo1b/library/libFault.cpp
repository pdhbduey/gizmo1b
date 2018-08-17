#include "mibspi.h"
#include "libFault.h"
#include "libDelay.h"
#include "libWrapGioPortA.h"
#include "libWrapMibSpi5.h"

LibFault::LibFault() :
    m_drvErrClr(new LibWrapMibSpi5, PIN_ENA) // 97:MIBSPI5NENA:DRV_ERR_CLR
{
    m_ntcMap[NTC1] = new LibWrapGioPort::Port(new LibWrapMibSpi5, PIN_CS0); // 32:MIBSPI5NCS[0]:NTC_PRESENT1
    m_ntcMap[NTC2] = new LibWrapGioPort::Port(new LibWrapGioPortA, 2);      //  9:GIOA[2]:NTC_PRESENT2
    m_faultMap[TEC_OCD_POS] = 0;
    m_faultMap[TEC_OCD_NEG] = 0;
    m_faultMap[OVERTEMP1]   = 0;
    m_faultMap[OVERTEMP2]   = 0;
}

LibFault::~LibFault()
{
}

void LibFault::reset()
{
    m_drvErrClr.m_libWrapGioPort->setPin(m_drvErrClr.m_pin, true);
    LibDelay::pmuMicrosecDelay(5); // >1us
    m_drvErrClr.m_libWrapGioPort->setPin(m_drvErrClr.m_pin, false);
}

int LibFault::getFault(int fault, bool& isFault)
{
    if (m_faultMap.find(fault) == m_faultMap.end()) {
        return ERROR_INVALID_FAULT;
    }
    if (!m_faultMap[fault]) {
        return ERROR_NOT_IMPLEMENTED;
    }
    isFault = m_faultMap[fault]->m_libWrapGioPort->getPin(m_faultMap[fault]->m_pin);
    return OKAY;
}

int LibFault::getNtcPresent(int ntc, bool& isNtcPresent)
{
    if (m_ntcMap.find(ntc) == m_ntcMap.end()) {
        return ERROR_INVALID_NTC;
    }
    isNtcPresent = m_ntcMap[ntc]->m_libWrapGioPort->getPin(m_ntcMap[ntc]->m_pin);
    return OKAY;
}
