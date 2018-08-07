#ifndef _BOARD_TEST_GPIO_H_
#define _BOARD_TEST_GPIO_H_

#include "libDioIn.h"
#include "libDioOut.h"
#include "boardTest.h"

class BoardTestDio: public BoardTest
{
public:
    enum InPins {
        DIN_0_STATE = (uint32)((uint32)1U << 0U),
        DIN_1_STATE = (uint32)((uint32)1U << 1U),
        DIN_2_STATE = (uint32)((uint32)1U << 2U),
        DIN_3_STATE = (uint32)((uint32)1U << 3U),
        DIN_4_STATE = (uint32)((uint32)1U << 4U),
        DIN_5_STATE = (uint32)((uint32)1U << 5U),
        DIN_6_STATE = (uint32)((uint32)1U << 6U),
        DIN_7_STATE = (uint32)((uint32)1U << 7U),
        DIN_8_STATE = (uint32)((uint32)1U << 8U),
        DIN_9_STATE = (uint32)((uint32)1U << 9U),
    };
    enum OutPins {
        DOUT_0_ON  = (uint32)((uint32)1U <<  0U),
        DOUT_0_OFF = (uint32)((uint32)1U <<  1U),
        DOUT_1_ON  = (uint32)((uint32)1U <<  2U),
        DOUT_1_OFF = (uint32)((uint32)1U <<  3U),
        DOUT_2_ON  = (uint32)((uint32)1U <<  4U),
        DOUT_2_OFF = (uint32)((uint32)1U <<  5U),
        DOUT_3_ON  = (uint32)((uint32)1U <<  6U),
        DOUT_3_OFF = (uint32)((uint32)1U <<  7U),
        DOUT_4_ON  = (uint32)((uint32)1U <<  8U),
        DOUT_4_OFF = (uint32)((uint32)1U <<  9U),
        DOUT_5_ON  = (uint32)((uint32)1U << 10U),
        DOUT_5_OFF = (uint32)((uint32)1U << 11U),
        DOUT_6_ON  = (uint32)((uint32)1U << 12U),
        DOUT_6_OFF = (uint32)((uint32)1U << 13U),
        DOUT_7_ON  = (uint32)((uint32)1U << 14U),
        DOUT_7_OFF = (uint32)((uint32)1U << 15U),
    };
    BoardTestDio();
    virtual ~BoardTestDio();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibDioIn m_libDioIn;
    LibDioOut m_libDioOut;
};

#endif // _BOARD_TEST_GPIO_H_
