#include "libGpio.h"
#include "gio.h"
#include "het.h"

bool LibGpio::s_isInitialized = false;
struct LibCommon::Port LibGpio::s_inputPort[] = {
    [DIN_0] = { .port = hetPORT1, .pin = PIN_HET_6  }, //  38:HET1_06:DIG_IN_A0
    [DIN_1] = { .port = hetPORT1, .pin = PIN_HET_7  }, //  33:HET1_07:DIG_IN_A1
    [DIN_2] = { .port = hetPORT1, .pin = PIN_HET_8  }, // 106:HET1_08:DIG_IN_A2
    [DIN_3] = { .port = hetPORT1, .pin = PIN_HET_9  }, //  35:HET1_09:DIG_IN_A3
    [DIN_4] = { .port = hetPORT1, .pin = PIN_HET_10 }, // 118:HET1_10:DIG_IN_A4
    [DIN_5] = { .port = hetPORT1, .pin = PIN_HET_11 }, //   6:HET1_11:DIG_IN_A5
    [DIN_6] = { .port = hetPORT1, .pin = PIN_HET_16 }, // 139:HET1_16:DIG_IN_B0
    [DIN_7] = { .port = hetPORT1, .pin = PIN_HET_18 }, // 140:HET1_18:DIG_IN_B1
    [DIN_8] = { .port = hetPORT1, .pin = PIN_HET_20 }, // 141:HET1_20:DIG_IN_B2
    [DIN_9] = { .port = hetPORT1, .pin = PIN_HET_22 }, //  15:HET1_22:DIG_IN_B3
};
struct LibCommon::Port LibGpio::s_outputPort[] = {
    [DOUT_0] = { .port = hetPORT1, .pin = PIN_HET_12 }, // 124:HET1_12:DIG_OUT_A0
    [DOUT_1] = { .port = hetPORT1, .pin = PIN_HET_13 }, //  39:HET1_13:DIG_OUT_A1
    [DOUT_2] = { .port = hetPORT1, .pin = PIN_HET_14 }, // 125:HET1_14:DIG_OUT_A2
    [DOUT_3] = { .port = hetPORT1, .pin = PIN_HET_15 }, //  41:HET1_15:DIG_OUT_A3
    [DOUT_4] = { .port = hetPORT1, .pin = PIN_HET_24 }, //  91:HET1_24:DIG_OUT_B0
    [DOUT_5] = { .port = hetPORT1, .pin = PIN_HET_26 }, //  92:HET1_26:DIG_OUT_B1
    [DOUT_6] = { .port = hetPORT1, .pin = PIN_HET_28 }, // 107:HET1_28:DIG_OUT_B2
    [DOUT_7] = { .port = hetPORT1, .pin = PIN_HET_30 }, // 127:HET1_30:DIG_OUT_B3
};

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
    default:
        result = INVALID_OUTPUT_PIN;
        break;
    case DOUT_0:
    case DOUT_1:
    case DOUT_2:
    case DOUT_3:
    case DOUT_4:
    case DOUT_5:
    case DOUT_6:
    case DOUT_7:
        if (s_outputPort[pin].port) {
            gioSetBit(s_outputPort[pin].port, s_outputPort[pin].pin, set);
        }
        break;
    }
    return result;
}

int LibGpio::getPin(int pin, bool& isSet)
{
    int result = OKAY;
    switch (pin) {
    default:
        result = INVALID_INPUT_PIN;
        break;
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
        if (s_inputPort[pin].port) {
            isSet = gioGetBit(s_inputPort[pin].port, s_inputPort[pin].pin);
        }
        break;
    }
    return result;
}
