#ifndef _LIB_COMMON_H_
#define _LIB_COMMON_H_

#include "reg_gio.h"

namespace LibCommon {
    struct Port {
        gioPORT_t* port;
        int pin;
    };
}

#endif // _LIB_COMMON_H_
