#ifndef _BOARD_TEST_ADC_H_
#define _BOARD_TEST_ADC_H_

#include "boardTest.h"
#include "libAdc.h"

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
    BoardTestAdc();
    virtual ~BoardTestAdc();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibAdc m_libAdc;
    int m_channel;
    float m_result;
    int m_status;
};

#endif // _BOARD_TEST_ADC_H_
