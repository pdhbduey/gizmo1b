#pragma once

#include <map>
#include "libWrapGioPort.h"

class LibDioOut {
public:
    enum Pins {
        // Mapped A[0-3], B[0-3] --> [0-7]
        DOUT_0,
        DOUT_1,
        DOUT_2,
        DOUT_3,
        DOUT_4,
        DOUT_5,
        DOUT_6,
        DOUT_7,
    };
    enum Status {
        OKAY,
        INVALID_OUTPUT_PIN,
    };
public:
    LibDioOut();
    virtual ~LibDioOut();
    int setPin(int pin, bool set);
    int getPin(int pin, bool& isSet);
private:
    std::map<int, LibWrapGioPort::Port*> m_outPinsMap;
};
