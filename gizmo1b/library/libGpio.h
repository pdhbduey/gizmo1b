#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

#include "libCommon.h"

class LibGpio {
public:
    enum Input {
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
        INPUT_MAX

    };
    enum Output {
        // Mapped A[0-3], B[0-3] --> [0-7]
        DOUT_0,
        DOUT_1,
        DOUT_2,
        DOUT_3,
        DOUT_4,
        DOUT_5,
        DOUT_6,
        DOUT_7,
        OUTPUT_MAX
    };
    enum Status {
        OKAY,
        INVALID_INPUT_PIN,
        INVALID_OUTPUT_PIN,
    };
public:
    LibGpio();
    virtual ~LibGpio();
    int setPin(int pin, bool set);
    int getPin(int pin, bool& isSet);
private:
    static bool s_isInitialized;
    static struct LibCommon::Port s_inputPort[INPUT_MAX];
    static struct LibCommon::Port s_outputPort[OUTPUT_MAX];
};

#endif // _LIB_GPIO_H_
