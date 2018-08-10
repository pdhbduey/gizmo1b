#include "boardTestLed.h"
#include "libDelay.h"

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
        value |= (isSet ? RED_OFF : RED_ON);
        m_libLed.get(LibLed::GREEN, isSet);
        value |= (isSet ? GREEN_OFF : GREEN_ON);
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
            m_libLed.set(LibLed::RED, false);
        }
        if (value & RED_OFF) {
            m_libLed.set(LibLed::RED, true);
        }
        if (value & GREEN_ON) {
            m_libLed.set(LibLed::GREEN, false);
        }
        if (value & GREEN_OFF) {
            m_libLed.set(LibLed::GREEN, true);
        }
        break;
    }
    return OKAY;
}

void BoardTestLed::test()
{
    BoardTestLed boardTestLed;
    boardTestLed.set(LED_CONTROL, RED_OFF | GREEN_ON);
    for (int i = 0; i < 10; i++) {
        uint32 value;
        boardTestLed.get(LED_CONTROL, value);
        boardTestLed.set(LED_CONTROL, ~value);
        LibDelay::waitForTimer(500000);
    }
    boardTestLed.set(LED_CONTROL, RED_OFF | GREEN_OFF);
}
