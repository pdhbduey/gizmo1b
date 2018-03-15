#include "libLed.h"
#include "gio.h"
#include "mibspi.h"

bool LibLed::s_isInitialized = false;
struct LibCommon::Port LibLed::s_ledPort[] = {
    [GREEN] = { .port = mibspiPORT1, .pin = PIN_ENA }, //  96:MIBSPI1NENA:DEBUG_GREEN_LED
    [RED]   = { .port = mibspiPORT1, .pin = PIN_CS0 }, // 105:MIBSPI1NCS[0]:ERROR_RED_LED
};

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
        if (s_ledPort[led].port) {
            gioSetBit(s_ledPort[led].port, s_ledPort[led].pin, set);
        }
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
        if (s_ledPort[led].port) {
            isSet = gioGetBit(s_ledPort[led].port, s_ledPort[led].pin);
        }
        break;
    }
    return result;
}
