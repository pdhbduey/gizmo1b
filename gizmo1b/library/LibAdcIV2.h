#pragma once

#include <LibADS8330.h>

class LibAdcIV2 : public LibADS8330
{
public:
    enum Inputs {
        ISENSE = LibAdc::CHANNEL_0,
        VSENSE = LibAdc::CHANNEL_1,
    };
public:
    LibAdcIV2();
protected:
    void initialize();
    virtual int read(int channel, uint32& value);
protected:
    uint32 m_isenseCorrection;
};
