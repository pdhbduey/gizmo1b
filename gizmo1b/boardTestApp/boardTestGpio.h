#ifndef _BOARD_TEST_GPIO_H_
#define _BOARD_TEST_GPIO_H_

#include "boardTest.h"
#include "libGpio.h"

class BoardTestGpio: public BoardTest
{
public:
    enum InBits {
        DIN_0_STATE = (uint32)((uint32)1U << 0U),
        DIN_1_STATE = (uint32)((uint32)1U << 1U),
        DIN_2_STATE = (uint32)((uint32)1U << 2U),
        DIN_3_STATE = (uint32)((uint32)1U << 3U),
        DIN_4_STATE = (uint32)((uint32)1U << 4U),
        DIN_5_STATE = (uint32)((uint32)1U << 5U),
    };
    enum OutBits {
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
    BoardTestGpio();
    virtual ~BoardTestGpio();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibGpio m_libGpio;
};

#endif // _BOARD_TEST_GPIO_H_
