#ifndef _BOARD_TEST_TEC_H_
#define _BOARD_TEST_TEC_H_

#include "boardTest.h"
#include "libLed.h"
#include "libTec.h"

class BoardTestTec: public BoardTest
{
public:
    enum Control {
        DISABLE                     = (uint32)((uint32) 1U << 0U),
        ENABLE                      = (uint32)((uint32) 1U << 1U),
        START_WAVEFORM              = (uint32)((uint32) 1U << 2U),
        STOP_WAVEFORM               = (uint32)((uint32) 1U << 3U),
        CLOSED_LOOP_DISABLE         = (uint32)((uint32) 1U << 4U),
        CLOSED_LOOP_ENABLE          = (uint32)((uint32) 1U << 5U),
        CUSTOM_WAVEFORM_RESET_INDEX = (uint32)((uint32) 1U << 6U),
        CUSTOM_WAVEFORM_INC_INDEX   = (uint32)((uint32) 1U << 7U),
    };
    enum SnaphotControl {
        SNAPSHOT_START = (uint32)((uint32) 1U << 0U),
        SNAPSHOT_STOP  = (uint32)((uint32) 1U << 1U),
    };
public:
    BoardTestTec();
    virtual ~BoardTestTec();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibTec m_libTec;
    int m_status;
    std::vector<struct LibTec::IrefSample> m_customWaveform;
    struct LibTec::IrefSample m_irefSample;
    uint32 m_customeWaveformCycles;
    LibLed m_libLed;
    int m_snapshotStatus;
};

#endif // _BOARD_TEST_TEC_H_
