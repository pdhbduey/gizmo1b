// launchxl2-tms570ls1224
#ifndef _BOARD_TEST_USER_LED_H_
#define _BOARD_TES_USER_LED_H_

#include "boardTest.h"
#include "libUserLed.h"

class BoardTestUserLed : public BoardTest
{
public:
    enum Bits {
        LED_A_ON  = (uint32)((uint32)1U << 0U),
        LED_A_OFF = (uint32)((uint32)1U << 1U),
        LED_B_ON  = (uint32)((uint32)1U << 2U),
        LED_B_OFF = (uint32)((uint32)1U << 3U),
    };
public:
    BoardTestUserLed();
    virtual ~BoardTestUserLed();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibUserLed m_libUserLed;
};

#endif // _BOARD_TES_USER_LED_H_
