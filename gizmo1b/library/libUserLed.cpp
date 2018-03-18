#include "libUserLed.h"
#include "libWrapGioPortB.h"

LibUserLed::LibUserLed()
{
    LibWrapGioPort* libWrapGioPortB = new LibWrapGioPortB;
    m_userLedMap[USER_LED_AOUT] = new LibWrapGioPort::Port(libWrapGioPortB, 1);
    m_userLedMap[USER_LED_BOUT] = new LibWrapGioPort::Port(libWrapGioPortB, 2);
}

LibUserLed::~LibUserLed()
{
}

 bool LibUserLed::isLedValid(int led)
 {
     return m_userLedMap.find(led) != m_userLedMap.end() && m_userLedMap[led];
 }

int LibUserLed::set(int led, bool set)
{
    if (!isLedValid(led)) {
        return INVALID_PIN;
    }
    m_userLedMap[led]->m_libWrapGioPort->setPin(m_userLedMap[led]->m_pin, set);
    return OKAY;
}

int LibUserLed::get(int led, bool& isSet)
{
    if (!isLedValid(led)) {
        return INVALID_PIN;
    }
    isSet = m_userLedMap[led]->m_libWrapGioPort->getPin(m_userLedMap[led]->m_pin);
    return OKAY;
}
