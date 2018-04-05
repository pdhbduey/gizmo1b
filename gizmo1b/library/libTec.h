#ifndef _LIB_TEC_H_
#define _LIB_TEC_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "libWrapGioPort.h"
#include "libAdc.h"
#include "libDac.h"
#include "libTask.h"

// Make sure to start LibTech thread after creating LibTec object by calling LibTask::start() method.
class LibTec : public LibTask
{
public:
    typedef std::pair<int, float> TimeCurrent;
    enum Status {
       OKAY,
       ERROR_ISENSE,
       ERROR_VSENSE,
       ERROR_REF_CURRENT_OUT_OF_RANGE,
       ERROR_SET_REF_CURRENT,
       ERROR_WAVEFORM_TYPE_OUT_OF_RANGE,
       ERROR_WAVEFORM_PERIOD_OUT_OF_RANGE,
       ERROR_DAC_OFFSET_OUT_OF_RANGE,
       ERROR_PROPORTIONAL_GAIN_OUT_OF_RANGE,
       ERROR_INTEGRAL_GAIN_OUT_OF_RANGE,
       ERROR_DERIVATIVE_GAIN_OUT_OF_RANGE,
    };
    enum WaveformType {
        WAVEFORM_TYPE_CONSTANT,
        WAVEFORM_TYPE_SINUSOIDAL,
        WAVEFORM_TYPE_TRIANGULAR,
        WAVEFORM_TYPE_SQUARE,
    };
public:
    LibTec(const char* name = "LibTec");
    virtual ~LibTec();
    void enable(bool en);
    bool isEnabled();
    int getISense(float& iSense); // Calculated Current Based on ADC voltage
    int getVSense(float& vSense); // Calculated Voltage Based on ADC voltage
    // Calculated Closed Loop Current Output (Iref)
    int setRefCurrent(float refCurrent); // [-15A,15A]
    float getRefCurrent();
    int setWaveformType(uint32 waveformType);
    uint32 getWaveformType();
    int setWaveformPeriod(uint32 waveformPeriod); // 2ms-10,000ms
    uint32 getWaveformPeriod();
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
    int setOffset(float offset); // -1.0-1.0
    float getOffset();
    bool isClosedLoopEnabled();
private:
    enum adcChannels {
        ISENSE = LibAdc::CHANNEL_1,
        VSENSE = LibAdc::CHANNEL_0,
    };
    struct WaveformSample {
        float m_ticks;
        float m_value;
    };
private:
    virtual void run();
    bool driveControl(float control);
private:
    LibWrapGioPort::Port m_tecEnable;
    LibAdc m_libAdc;
    LibDac m_libDac;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    uint32 m_waveformType;
    struct WaveformSample m_waveform[1000];
    uint32 m_waveformPeriod;
    TickType_t m_ticks;
    float m_prevError;
    float m_accError;
    bool m_isCloseLoopInitialized;
    bool m_isWaveformRunning;
    float m_refCurrent;
    float m_pidProportionalGain;
    float m_pidIntegralGain;
    float m_pidDerivativeGain;
    float m_offset;
    bool m_isClosedLoopEnabled;
    bool m_isEnabled;
};

#endif // _LIB_TEC_H_
