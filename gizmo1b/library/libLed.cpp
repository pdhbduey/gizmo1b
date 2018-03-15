#include "libLed.h"
#include "gio.h"
#include "mibspi.h"

bool LibLed::s_isInitialized = false;

LibLed::LibLed()
{
    if (!s_isInitialized) {
        mibspiInit();
        s_isInitialized = true;
    }
}

LibLed::~LibLed()
{
}

int LibLed::set(int led, bool set)
{
    uint32 result = OKAY;
    switch (led) {
    default:
        result = INVALID_LED_PIN;
        break;
    case RED:
    case GREEN:
        gioSetBit(mibspiPORT1, led, set);
        break;
    }
    return result;
}

int LibLed::get(int led, bool& isSet)
{
    int result = OKAY;
    switch (led) {
    default:
        result = INVALID_LED_PIN;
        break;
    case RED:
    case GREEN:
        isSet = gioGetBit(mibspiPORT1, led);
        break;
    }
    return result;
}
