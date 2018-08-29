#pragma once

#include <sys_common.h>

class LibDac
{
public:
    enum Status {
        OKAY,
        ERROR_SET_VALUE_OUT_OF_RANGE,
        ERROR_TIME_OUT,
    };
public:
    virtual int set(float value)  = 0;
    virtual int set(uint32 value) = 0;
    float get();
protected:
    float m_value;
};
