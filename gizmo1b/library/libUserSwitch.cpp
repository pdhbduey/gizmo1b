#include "libUserSwitch.h"
#include "libWrapMibSpi1.h"
#include "libWrapMibSpi3.h"
#include "mibspi.h"

LibUserSwitch::LibUserSwitch()
{
    m_userSwitchMap[USER_SWITCH_A] = new LibWrapGioPort::Port(new LibWrapMibSpi3, PIN_CS0);
    m_userSwitchMap[USER_SWITCH_B] = new LibWrapGioPort::Port(new LibWrapMibSpi1, PIN_CS4);
}

LibUserSwitch::~LibUserSwitch()
{
}

 bool LibUserSwitch::isSwitchValid(int sw)
 {
     return m_userSwitchMap.find(sw) != m_userSwitchMap.end()
         && m_userSwitchMap[sw];
 }

int LibUserSwitch::get(int sw, bool& isSet)
{
    if (!isSwitchValid(sw)) {
        return INVALID_PIN;
    }
    isSet = m_userSwitchMap[sw]->m_libWrapGioPort->getPin(m_userSwitchMap[sw]->m_pin);
    return OKAY;
}
