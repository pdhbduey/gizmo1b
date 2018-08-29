#pragma once

#include <LibLed.h>
#include <BoardTest.h>

class BoardTestLed: public BoardTest
{
public:
    enum Control {
        RED_ON     = (uint32)((uint32)1U << 0U),
        RED_OFF    = (uint32)((uint32)1U << 1U),
        GREEN_ON   = (uint32)((uint32)1U << 2U),
        GREEN_OFF  = (uint32)((uint32)1U << 3U),
    };
public:
    BoardTestLed(LibLed* libLed);
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibLed& m_libLed;
};
