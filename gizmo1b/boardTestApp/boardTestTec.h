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
        RESET_CURRENT_COUNTER  = (uint32)((uint32) 1U << 2U),
        START_CURRENT_WAVEFORM = (uint32)((uint32) 1U << 3U),
        STOP_CURRENT_WAVEFORM  = (uint32)((uint32) 1U << 4U),
    };
public:
    BoardTestTec();
    virtual ~BoardTestTec();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
    static void test();
private:
    LibTec m_libTec;
    std::vector<LibTec::TimeCurrent> m_setPoints;
    bool m_isEnabled;
    int m_timeValue;
    float m_currentValue;
    int m_loopCount;
    int m_status;
};

#endif // _BOARD_TEST_TEC_H_
