#include "FreeRTOS.h"
#include "os_task.h"
#include "libFault.h"
#include "mibspi.h"
#include "libWrapGioPortA.h"
#include "libWrapMibSpi5.h"

LibFault::LibFault()
{
    LibWrapGioPort* libWrapMibSpi5 = new LibWrapMibSpi5;
    LibWrapGioPort* libWrapGioPortA = new LibWrapGioPortA;
    m_ntcMap[NTC1] = new LibWrapGioPort::Port(libWrapMibSpi5, PIN_CS0);  // 32:MIBSPI5NCS[0]:NTC_PRESENT1
    m_ntcMap[NTC2] = new LibWrapGioPort::Port(libWrapGioPortA, 2);       //  9:GIOA[2]:NTC_PRESENT2
    m_resetMap[RESET_FAULT] = new LibWrapGioPort::Port(libWrapMibSpi5, PIN_ENA);  // 97:MIBSPI5NENA:DRV_ERR_CLR
}

LibFault::~LibFault()
{
}

void LibFault::reset()
{
    if (m_resetMap.find(RESET_FAULT) != m_resetMap.end()
     && m_resetMap[RESET_FAULT]) {
        m_resetMap[RESET_FAULT]->m_libWrapGioPort->setPin(m_resetMap[RESET_FAULT]->m_pin, true);
        vTaskDelay(pdMS_TO_TICKS(1)); // >1us
        m_resetMap[RESET_FAULT]->m_libWrapGioPort->setPin(m_resetMap[RESET_FAULT]->m_pin, false);
    }
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
