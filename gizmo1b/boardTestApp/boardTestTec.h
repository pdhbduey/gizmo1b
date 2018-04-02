#ifndef _BOARD_TEST_TEC_H_
#define _BOARD_TEST_TEC_H_

#include "boardTest.h"
#include "libTec.h"

class BoardTestTec: public BoardTest
{
public:
    enum Control {
        DISABLE             = (uint32)((uint32) 1U << 0U),
        ENABLE              = (uint32)((uint32) 1U << 1U),
        START_WAVEFORM      = (uint32)((uint32) 1U << 2U),
        STOP_WAVEFORM       = (uint32)((uint32) 1U << 3U),
        CLOSED_LOOP_DISABLE = (uint32)((uint32) 1U << 4U),
        CLOSED_LOOP_ENABLE  = (uint32)((uint32) 1U << 5U),
    };
public:
    BoardTestTec();
    virtual ~BoardTestTec();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
    static void test();
private:
    LibTec m_libTec;
    int m_status;
};

#endif // _BOARD_TEST_TEC_H_
