#pragma once

#include <sys_common.h>

class LibAdc
{
public:
    enum Channel {
        CHANNEL_0,
        CHANNEL_1,
        CHANNEL_2,
        CHANNEL_3,
        CHANNEL_4,
        CHANNEL_5,
        CHANNEL_6,
        CHANNEL_7,
    };
    enum Status {
        OKAY,
        ERROR_INVALID_CHANNEL,
        ERROR_TIME_OUT,
    };
public:
    LibAdc(float vref);
    int read(int channel, float& value);              // 0-Vref
    virtual int read(int channel, uint32& value) = 0; // 0-65535
    bool isChannelCorrect(int channel);
protected:
    float m_vref;
};
