#include "boardTestLed.h"

BoardTestLed::BoardTestLed()
{
}

BoardTestLed::~BoardTestLed()
{
}

int BoardTestLed::get(uint32 address, uint32& value)
{
    bool isSet;
    switch (address) {
    default:
        return ERROR_ADDR;
    case LED_CONTROL:
        value = 0;
        m_libLed.get(LibLed::RED, isSet);
        value |= (isSet ? RED_ON : RED_OFF);
        m_libLed.get(LibLed::GREEN, isSet);
        value |= (isSet ? GREEN_ON : GREEN__OFF);
        break;
    }
    return OKAY;
}

int BoardTestLed::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case LED_CONTROL:
        if (value & RED_ON) {
            m_libLed.set(LibLed::RED, true);
        }
        if (value & RED_OFF) {
            m_libLed.set(LibLed::RED, false);
        }
        if (value & GREEN_ON) {
            m_libLed.set(LibLed::GREEN, true);
        }
        if (value & GREEN__OFF) {
            m_libLed.set(LibLed::GREEN, false);
        }
        break;
    }
    return OKAY;
}
