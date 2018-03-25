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
}

LibFault::~LibFault()
{
}

void LibFault::reset()
{
    m_drvErrClr.m_libWrapGioPort->setPin(m_drvErrClr.m_pin, true);
    LibDelay::us(100); // >1us
    m_drvErrClr.m_libWrapGioPort->setPin(m_drvErrClr.m_pin, false);
}

int LibFault::readState(int state, bool& isFault)
{
    if (m_stateMap.find(state) == m_stateMap.end() || !m_stateMap[state]) {
        return INVALID_STATE;
    }
    isFault = m_stateMap[state]->m_libWrapGioPort->getPin(m_stateMap[state]->m_pin);
    return OKAY;
}

int LibFault::readNtcPresent(int ntc, bool& isNtcPresent)
{
    if (m_ntcMap.find(ntc) == m_ntcMap.end() || !m_ntcMap[ntc]) {
        return INVALID_NTC;
    }
    isNtcPresent = m_ntcMap[ntc]->m_libWrapGioPort->getPin(m_ntcMap[ntc]->m_pin);
    return OKAY;
}
