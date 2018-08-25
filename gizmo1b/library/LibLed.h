#pragma once

#include <map>
#include <libWrapGioPort.h>

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
    int set(int led, bool set);
    int get(int led, bool& isSet);
private:
    bool isLedValid(int led);
protected:
    std::map<int, LibWrapGioPort::Port*> m_ledMap;
};
