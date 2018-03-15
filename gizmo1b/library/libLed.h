#ifndef _LIB_LED_H_
#define _LIB_LED_H_

#include "sys_common.h"

class LibLed
{
public:
    enum Control {
        GREEN = (uint32)((uint32)1U << 8U), //  96:MIBSPI1NENA:DEBUG_GREEN_LED
        RED   = (uint32)((uint32)1U << 0U), // 105:MIBSPI1NCS[0]:ERROR_RED_LED
    };
    enum Status {
        OKAY,
        INVALID_LED_PIN,
    };
    LibLed();
    virtual ~LibLed();
    int set(int led, bool set);
    int get(int led, bool& isSet);
private:
    static bool s_isInitialized;
};

#endif // _LIB_LED_H_
