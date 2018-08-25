#pragma once

#include <LibAdc.h>
#include <BoardTest.h>

class BoardTestAdc: public BoardTest
{
public:
    enum ControlMask {
        ADC_CHANNEL_MASK = 7,
        ADC_START_MASK   = 1,
    };
    enum ControlShift {
        ADC_CHANNEL_SHIFT      = 0,
        ADC_START_MASK_SHIFT   = 8,
    };
    enum StatusShift {
        ADC_STATUS_SHIFT = 0,
    };
public:
    BoardTestAdc(LibAdc* libAdc);
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibAdc& m_libAdc;
    int m_channel;
    float m_result;
    int m_status;
};
