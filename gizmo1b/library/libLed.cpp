#include "libLed.h"
#include "mibspi.h"
#include "libWrapMibSpi1.h"

LibLed::LibLed()
{
    LibWrapGioPort* libWrapMibSpi1 = new LibWrapMibSpi1;
    m_ledMap[GREEN] = new LibWrapGioPort::Port(libWrapMibSpi1, PIN_ENA);  //  96:MIBSPI1NENA:DEBUG_GREEN_LED
    m_ledMap[RED]   = new LibWrapGioPort::Port(libWrapMibSpi1, PIN_CS0);  // 105:MIBSPI1NCS[0]:ERROR_RED_LED
}

LibLed::~LibLed()
{
}

 bool LibLed::isLedValid(int led)
 {
     return m_ledMap.find(led) != m_ledMap.end() && m_ledMap[led];
 }

int LibLed::set(int led, bool set)
{
    if (!isLedValid(led)) {
        return INVALID_LED_PIN;
    }
    m_ledMap[led]->m_libWrapGioPort->setPin(m_ledMap[led]->m_pin, set);
    return OKAY;
}

int LibLed::get(int led, bool& isSet)
{
    if (!isLedValid(led)) {
        return INVALID_LED_PIN;
    }
    isSet = m_ledMap[led]->m_libWrapGioPort->getPin(m_ledMap[led]->m_pin);
    return OKAY;
}
