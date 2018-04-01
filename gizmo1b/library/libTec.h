#ifndef _LIB_TEC_H_
#define _LIB_TEC_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "libWrapGioPort.h"
#include "libAdc.h"
#include "libDac.h"

class LibTec
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
    };
    enum WaveformType {
        WAVEFORM_TYPE_CONSTANT,
        WAVEFORM_TYPE_SINUSOIDAL,
        WAVEFORM_TYPE_TRIANGULAR,
        WAVEFORM_TYPE_SQUARE,
    };
public:
    LibTec();
    virtual ~LibTec();
    void enable(bool en);
    int getISense(float& iSense); // Calculated Current Based on ADC voltage
    int getVSense(float& vSense); // Calculated Voltage Based on ADC voltage
    // Calculated Closed Loop Current Output (Iref)
    int setRefCurrent(float refCurrent); // [-15A,15A]
    float getRefCurrent();
    int setWaveformType(uint32 waveformType);
    uint32 getWaveformType();
    int setWaveformPeriod(uint32 waveformPeriod);
    uint32 getWaveformPeriod();
    void waveformStart();
    void waveformStop();
private:
    enum adcChannels {
        ISENSE = LibAdc::CHANNEL_1,
        VSENSE = LibAdc::CHANNEL_0,
    };
    struct WaveformSample {
        float time;
        float value;
    };
private:
    LibWrapGioPort::Port m_tecEnable;
    LibAdc m_libAdc;
    LibDac m_libDac;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    uint32 m_waveformType;
    struct WaveformSample m_waveform[100];
    uint32 m_waveformPeriod; // 500ms-10,000ms
};

#endif // _LIB_TEC_H_
