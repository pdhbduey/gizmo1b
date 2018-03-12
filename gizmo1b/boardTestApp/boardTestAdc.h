#ifndef _BOARD_TEST_ADC_H_
#define _BOARD_TEST_ADC_H_

#include "boardTest.h"
#include "libAdc.h"

class BoardTestAdc: public BoardTest
{
public:
    BoardTestAdc();
    virtual ~BoardTestAdc();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    enum BitsAdcControlMask {
        ADC_CHANNEL_SELECT_MASK = (uint32)((uint32)7U << 0U),
        START_ADC_MASK          = (uint32)((uint32)1U << 8U),
    };
    enum BitsAdcControlShift {
        ADC_CHANNEL_SELECT_SHIFT = 0,
        START_ADC_SHIFT          = 8,
    };
    enum BitsAdcStatusMask {
        ADC_STATUS_MASK = (uint32)((uint32)3U << 0U),
    };
    enum BitsAdcStatusShift {
        ADC_STATUS_SHIFT = 0,
    };
    LibAdc m_libAdc;
    int m_channel;

};

#endif // _BOARD_TEST_ADC_H_
