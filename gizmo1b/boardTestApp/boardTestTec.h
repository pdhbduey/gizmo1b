#ifndef _BOARD_TEST_TEC_H_
#define _BOARD_TEST_TEC_H_

#include "boardTest.h"
#include "libTec.h"

class BoardTestTec: public BoardTest
{
public:
    enum Control {
        DISABLE                = (uint32)((uint32) 1U << 0U),
        ENABLE                 = (uint32)((uint32) 1U << 1U),
        START_WAVEFORM = (uint32)((uint32) 1U << 2U),
        STOP_WAVEFORM  = (uint32)((uint32) 1U << 3U),
    };
public:
    BoardTestTec();
    virtual ~BoardTestTec();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
    static void test();
private:
    LibTec m_libTec;
    bool m_isEnabled;
    int m_status;
    bool m_isWaveformStarted;
};

#endif // _BOARD_TEST_TEC_H_
