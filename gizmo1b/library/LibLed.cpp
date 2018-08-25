#include <LibLed.h>

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
