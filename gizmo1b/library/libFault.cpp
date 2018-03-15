#include "FreeRTOS.h"
#include "os_task.h"
#include "libFault.h"
#include "gio.h"
#include "mibspi.h"


bool LibFault::s_isInitialized = false;
struct LibCommon::Port LibFault::s_statePort[STATE_MAX];
struct LibCommon::Port LibFault::s_ntcPort[] = {
    [NTC1] = { .port = mibspiPORT5, .pin = PIN_CS0 }, // 32:MIBSPI5NCS[0]:NTC_PRESENT1
    [NTC2] = { .port = gioPORTA,    .pin = 2 },       //  9:GIOA[2]:NTC_PRESENT2
};
struct LibCommon::Port LibFault::s_resetPort[] = {
    [RESET_FAULT] = { .port = mibspiPORT5, .pin = PIN_ENA }, // 97:MIBSPI5NENA:DRV_ERR_CLR
};

LibFault::LibFault()
{
    if (!s_isInitialized) {
        gioInit();
        mibspiInit();
        s_isInitialized = true;
    }
}

LibFault::~LibFault()
{
}

void LibFault::reset()
{
    if (s_resetPort[RESET_FAULT].port) {
        gioSetBit(s_resetPort[RESET_FAULT].port, s_resetPort[RESET_FAULT].pin, true);
        vTaskDelay(pdMS_TO_TICKS(1)); // >1us
        gioSetBit(s_resetPort[RESET_FAULT].port, s_resetPort[RESET_FAULT].pin, false);
    }
}

int LibFault::readState(int state, bool& isFault)
{
    switch (state) {
    case TEC_OCD_POS:
    case TEC_OCD_NEG:
    case OVERTEMP1:
    case OVERTEMP2:
        if (s_statePort[state].port) {
            isFault = gioGetBit(s_statePort[state].port, s_statePort[state].pin);
        }
        break;
    default:
        return INVALID_STATE;
    }
    return OKAY;
}

int LibFault::readNtcPresent(int ntc, bool& isNtcPresent)
{
    switch (ntc) {
    case NTC1:
    case NTC2:
        if (s_ntcPort[ntc].port) {
            isNtcPresent = gioGetBit(s_ntcPort[ntc].port, s_ntcPort[ntc].pin);
        }
        break;
    default:
        return INVALID_NTC;
    }
    return OKAY;
}
