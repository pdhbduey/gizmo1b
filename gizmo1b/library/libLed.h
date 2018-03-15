#ifndef _LIB_LED_H_
#define _LIB_LED_H_

#include "libCommon.h"

class LibLed
{
public:
    enum Control {
        GREEN,// = (uint32)((uint32)1U << 8U), //  96:MIBSPI1NENA:DEBUG_GREEN_LED
        RED, //   = (uint32)((uint32)1U << 0U), // 105:MIBSPI1NCS[0]:ERROR_RED_LED
        LED_MAX
    };
    enum Status {
        OKAY,
        INVALID_LED_PIN,
    };
public:
    LibLed();
    virtual ~LibLed();
    int set(int led, bool set);
    int get(int led, bool& isSet);
private:
    static bool s_isInitialized;
    static struct LibCommon::Port s_ledPort[LED_MAX];
};

#endif // _LIB_LED_H_
