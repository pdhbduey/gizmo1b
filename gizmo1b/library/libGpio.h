#ifndef _LIB_GPIO_H_
#define _LIB_GPIO_H_

#include "sys_common.h"

class LibGpio {
public:
    enum Input {
        // Mapped A[0-5], B[0-3] --> [0-9]
        DIN_0 = (uint32)((uint32)1U <<  6U), //  38:HET1_06:DIG_IN_A0
        DIN_1 = (uint32)((uint32)1U <<  7U), //  33:HET1_07:DIG_IN_A1
        DIN_2 = (uint32)((uint32)1U <<  8U), // 106:HET1_08:DIG_IN_A2
        DIN_3 = (uint32)((uint32)1U <<  9U), //  35:HET1_09:DIG_IN_A3
        DIN_4 = (uint32)((uint32)1U << 10U), // 118:HET1_10:DIG_IN_A4
        DIN_5 = (uint32)((uint32)1U << 11U), //   6:HET1_11:DIG_IN_A5
        DIN_6 = (uint32)((uint32)1U << 16U), // 139:HET1_16:DIG_IN_B0
        DIN_7 = (uint32)((uint32)1U << 18U), // 140:HET1_18:DIG_IN_B1
        DIN_8 = (uint32)((uint32)1U << 20U), // 141:HET1_20:DIG_IN_B2
        DIN_9 = (uint32)((uint32)1U << 22U), //  15:HET1_22:DIG_IN_B3

    };
    enum Output {
        // Mapped A[0-3], B[0-3] --> [0-7]
        DOUT_0 = (uint32)((uint32)1U << 12U), // 124:HET1_12:DIG_OUT_A0
        DOUT_1 = (uint32)((uint32)1U << 13U), //  39:HET1_13:DIG_OUT_A1
        DOUT_2 = (uint32)((uint32)1U << 14U), // 125:HET1_14:DIG_OUT_A2
        DOUT_3 = (uint32)((uint32)1U << 15U), //  41:HET1_15:DIG_OUT_A3
        DOUT_4 = (uint32)((uint32)1U << 24U), //  91:HET1_24:DIG_OUT_B0
        DOUT_5 = (uint32)((uint32)1U << 26U), //  92:HET1_26:DIG_OUT_B1
        DOUT_6 = (uint32)((uint32)1U << 28U), // 107:HET1_28:DIG_OUT_B2
        DOUT_7 = (uint32)((uint32)1U << 30U), // 127:HET1_30:DIG_OUT_B3
    };
    enum Status {
        OKAY,
        INVALID_INPUT_PIN,
        INVALID_OUTPUT_PIN,
    };
    LibGpio();
    virtual ~LibGpio();
    int setPin(int pin, bool set);
    int getPin(int pin, bool& isSet);
private:
    static bool s_isInitialized;
};

#endif // _LIB_GPIO_H_
