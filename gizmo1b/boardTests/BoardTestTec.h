#pragma once

#include <BoardTest.h>
#include <LibDac.h>
#include <LibLed.h>
#include <LibAdc.h>
#include <LibThermistor.h>
#include <libTec.h>

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
        SNAPSHOT_START  = (uint32)((uint32) 1U << 0U),
        SNAPSHOT_STOP   = (uint32)((uint32) 1U << 1U),
        SNAPSHOT_RAW    = (uint32)((uint32) 1U << 2U),
    };
    enum TraceControl {
        TRACE_START = (uint32)((uint32) 1U << 0U),
        TRACE_STOP  = (uint32)((uint32) 1U << 1U),
    };
    enum HeaterControl {
        HEATER_DISABLE                     = (uint32)((uint32) 1U << 0U),
        HEATER_ENABLE                      = (uint32)((uint32) 1U << 1U),
        HEATER_START_WAVEFORM              = (uint32)((uint32) 1U << 2U),
        HEATER_STOP_WAVEFORM               = (uint32)((uint32) 1U << 3U),
        HEATER_CLOSED_LOOP_DISABLE         = (uint32)((uint32) 1U << 4U),
        HEATER_CLOSED_LOOP_ENABLE          = (uint32)((uint32) 1U << 5U),
        HEATER_CUSTOM_WAVEFORM_RESET_INDEX = (uint32)((uint32) 1U << 6U),
        HEATER_CUSTOM_WAVEFORM_INC_INDEX   = (uint32)((uint32) 1U << 7U),
    };
public:
    BoardTestTec(LibTec* libTec, LibLed* libLed);
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibTec& m_libTec;
    int m_status;
    std::vector<struct LibTec::Sample> m_customWaveform;
    struct LibTec::Sample m_irefSample;
    uint32 m_customeWaveformCycles;
    LibLed& m_libLed;
    int m_snapshotStatus;
    int m_traceStatus;
    std::vector<struct LibTec::Sample> m_heaterCustomWaveform;
    struct LibTec::Sample m_heaterTrefSample;
    uint32 m_heaterCustomeWaveformCycles;
};
