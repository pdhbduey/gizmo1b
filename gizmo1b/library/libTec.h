#ifndef _LIB_TEC_H_
#define _LIB_TEC_H_

#include <vector>
#include <queue>
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "libWrapGioPort.h"
#include "libAdc.h"
#include "libDac.h"
#include "libTask.h"
#include "libThermistor.h"

// Make sure to start LibTech thread after creating LibTec object by calling LibTask::start() method.
class LibTec : public LibTask
{
public:
    struct IrefSample {
        uint32 m_time; // 0ms-10,000ms
        float m_iref;  // [-15A,15A]
        void clear();
    };
    enum Status {
       OKAY,
       ERROR_ISENSE,
       ERROR_VSENSE,
       ERROR_REF_CURRENT_OUT_OF_RANGE,
       ERROR_SET_REF_CURRENT,
       ERROR_WAVEFORM_TYPE_OUT_OF_RANGE,
       ERROR_WAVEFORM_PERIOD_OUT_OF_RANGE,
       ERROR_CUSTOM_WAVEFORM_EMPTY,
       ERROR_PROPORTIONAL_GAIN_OUT_OF_RANGE,
       ERROR_INTEGRAL_GAIN_OUT_OF_RANGE,
       ERROR_DERIVATIVE_GAIN_OUT_OF_RANGE,
       ERROR_CUSTOM_WAVEFORM_TIME_NOT_RISING,
       ERROR_CUSTOM_WAVEFORM_NON_ZERO_START_TIME,
    };
    enum WaveformType {
        WAVEFORM_TYPE_CONSTANT,
        WAVEFORM_TYPE_SINUSOIDAL,
        WAVEFORM_TYPE_TRIANGULAR,
        WAVEFORM_TYPE_SQUARE,
        WAVEFORM_TYPE_CUSTOM,
    };
    enum SnapshotStatus {
        SNAPSHOT_IN_PROGRESS                          = 1 << 0,
        ERROR_SNAPSHOT_SAMPLE_OUT_OF_RANGE            = 1 << 1,
        ERROR_SNAPSHOT_RESOLUTION_OUT_OF_RANGE        = 1 << 2,
        ERROR_SNAPSHOT_NUMBER_OF_SAMPLES_OUT_OF_RANGE = 1 << 3,
    };
    enum SnapshotRes {
        SNAPSHOT_RES_10,
        SNAPSHOT_RES_100,
        SNAPSHOT_RES_1000,
    };
public:
    LibTec(const char* name = "LibTec");
    virtual ~LibTec();
    void enable(bool en);
    bool isEnabled();
    int getISense(float& iSense);
    int getVSense(float& vSense);
    int setRefCurrent(float refCurrent); // [-15A,15A]
    float getRefCurrent();
    int setWaveformType(uint32 waveformType);
    uint32 getWaveformType();
    int setWaveformPeriod(uint32 waveformPeriod); // 2ms-10,000ms
    uint32 getWaveformPeriod();
    int setCustomWaveform(std::vector<struct IrefSample>& waveform, uint32 cycles); // cycles = 0 means cycle forever
    void getCustomWaveform(std::vector<struct IrefSample>& waveform, uint32& cycles);
    void waveformStart();
    void waveformStop();
    bool isWaveformStarted();
    void closedLoopEnable();
    void closedLoopDisable();
    int setProportionalGain(float gain); // 0.01-100
    float getProportionalGain();
    int setIntegralGain(float gain); // 0-100
    float getIntegrallGain();
    int setDerivativeGain(float gain); // 0-100
    float getDerivativeGain();
    bool isClosedLoopEnabled();
    void startSnaphot();
    void stopSnapshot();
    int setSnapshotNumberOfSamples(int nsamples);
    int getSnapshotNumberOfSamples();
    int setSnapshotResolution(int res);
    int getSnapshotResolution();
    bool isSnapshotRunning();
    int getSnapshotVsense(int sample, float& value);
    int getSnapshotIsense(int sample, float& value);
    int getSnapshotIref(int sample, float& value);
    int getSnapshotT1(int sample, float& value);
    int getSnapshotT2(int sample, float& value);
    int getSnapshotT3(int sample, float& value);
    int getSnapshotT4(int sample, float& value);
private:
    enum adcChannels {
        ISENSE = LibAdc::CHANNEL_1,
        VSENSE = LibAdc::CHANNEL_0,
    };
    struct WaveformSample {
        float m_ticks;
        float m_iref;
    };
private:
    virtual void run();
    bool driveControl(float control);
    float filter(float value);
    float getWaveformSample(TickType_t tick);
    bool isWaveformPeriodValid(uint32 waveformPeriod);
    bool isRefCurrentValid(float refCurrent);
    float getIrefFromCustomWaveform(std::vector<struct IrefSample>& waveform);
    uint32 getPeriodFromCustomWaveform(std::vector<struct IrefSample>& waveform);
    bool isCustomWaveformTimeRising(std::vector<struct IrefSample>& waveform);
    bool isCustomWaveformEmpty(std::vector<struct IrefSample>& waveform);
    bool isCustomWaveformStartTimeZero(std::vector<struct IrefSample>& waveform);
    bool isSnapshotSampleInRange(int sample);
private:
    LibWrapGioPort::Port m_tecEnable;
    LibAdc m_libAdc;
    LibDac m_libDac;
    LibThermistor m_libThermistor;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    uint32 m_waveformType;
    struct WaveformSample m_waveform[1000];
    uint32 m_waveformPeriod;
    TickType_t m_ticks;
    float m_prevError;
    float m_accError;
    bool m_isClosedLoopInitialized;
    bool m_isWaveformRunning;
    float m_refCurrent;
    float m_pidProportionalGain;
    float m_pidIntegralGain;
    float m_pidDerivativeGain;
    bool m_isClosedLoopEnabled;
    bool m_isEnabled;
    std::queue<float> m_filterQueue;
    std::vector<float> m_filterTaps;
    std::vector<struct IrefSample> m_customWaveform;
    uint32 m_cycles;
    static float* s_snapshotVsense;
    static float* s_snapshotIsense;
    static float* s_snapshotIref;
    static float* s_snapshotT1;
    static float* s_snapshotT2;
    static float* s_snapshotT3;
    static float* s_snapshotT4;
    bool m_isSnapshotRunning;
    int m_snapshotNumSamples;
    int m_snapshotRes;
    int m_snapShotSample;
};

#endif // _LIB_TEC_H_
