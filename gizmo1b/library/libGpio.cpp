#include "libGpio.h"
#include "gio.h"
#include "het.h"

bool LibGpio::s_isInitialized = false;

LibGpio::LibGpio()
{
    if (!s_isInitialized) {
        hetInit();
        s_isInitialized = true;
    }
}

LibGpio::~LibGpio()
{
}

int LibGpio::setPin(int pin, bool set)
{
    uint32 result = OKAY;
    switch (pin) {
    case DOUT_0:
    case DOUT_1:
    case DOUT_2:
    case DOUT_3:
    case DOUT_4:
    case DOUT_5:
    case DOUT_6:
    case DOUT_7:
        gioSetBit(hetPORT1, pin, set);
        break;
    default:
        result = INVALID_OUTPUT_PIN;
        break;
    }
    return result;
}

int LibGpio::getPin(int pin, bool& isSet)
{
    int result = OKAY;
    switch (pin) {
    case DIN_0:
    case DIN_1:
    case DIN_2:
    case DIN_3:
    case DIN_4:
    case DIN_5:
    case DIN_6:
    case DIN_7:
    case DIN_8:
    case DIN_9:
        isSet = gioGetBit(hetPORT1, pin);
        break;
    default:
        result = INVALID_INPUT_PIN;
        break;
    }
    return result;
}
