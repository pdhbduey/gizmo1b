#include <LibDelay.h>
#include <LibFault.h>

void LibFault::reset()
{
    m_drvErrClr->m_libWrapGioPort->setPin(m_drvErrClr->m_pin, true);
    LibDelay::pmuMicrosecDelay(5); // >1us
    m_drvErrClr->m_libWrapGioPort->setPin(m_drvErrClr->m_pin, false);
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
