#pragma once

#include <map>
#include "libWrapGioPort.h"

class LibDioIn {
public:
    enum Pins {
        // Mapped A[0-5], B[0-3] --> [0-9]
        DIN_0,
        DIN_1,
        DIN_2,
        DIN_3,
        DIN_4,
        DIN_5,
        DIN_6,
        DIN_7,
        DIN_8,
        DIN_9,

    };
    enum Status {
        OKAY,
        INVALID_INPUT_PIN,
    };
public:
    LibDioIn();
    virtual ~LibDioIn();
    int getPin(int pin, bool& isSet);
private:
    std::map<int, LibWrapGioPort::Port*> m_inPinsMap;
};
