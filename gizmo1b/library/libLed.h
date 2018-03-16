#ifndef _LIB_LED_H_
#define _LIB_LED_H_

#include <map>
#include "libWrapGioPort.h"

class LibLed
{
public:
    enum Control {
        GREEN,
        RED,
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
    bool isLedValid(int led);
private:
    std::map<int, LibWrapGioPort::Port*> m_ledMap;
};

#endif // _LIB_LED_H_
