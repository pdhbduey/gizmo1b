#ifndef _LIB_USER_LED_H_
#define _LIB_USER_LED_H_

#include <map>
#include "libWrapGioPort.h"

class LibUserLed
{
public:
    enum Control {
        USER_LED_AOUT,
        USER_LED_BOUT,
    };
    enum Status {
        OKAY,
        INVALID_PIN,
    };
public:
    LibUserLed();
    virtual ~LibUserLed();
    int set(int led, bool set);
    int get(int led, bool& isSet);
private:
    bool isLedValid(int led);
private:
    std::map<int, LibWrapGioPort::Port*> m_userLedMap;
};

#endif // _LIB_USER_LED_H_
