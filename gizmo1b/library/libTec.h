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
#include "libCircularBuffer.h"

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
    enum TraceRes {
        TRACE_RES_10,
        TRACE_RES_100,
    };
    enum TraceStatus {
        ERROR_TRACE_RESOLUTION_OUT_OF_RANGE             = 1 << 0,
        ERROR_TRACE_NUMBER_OF_READ_SAMPLES_OUT_OF_RANGE = 1 << 1,
    };
    enum HeaterStatus {
        HEATER_OKAY,
        ERROR_HEATER_TREF_OUT_OF_RANGE,
        ERROR_HEATER_IMAX_OUT_OF_RANGE,
        ERROR_HEATER_PROPORTIONAL_GAIN_OUT_OF_RANGE,
        ERROR_HEATER_INTEGRAL_GAIN_OUT_OF_RANGE,
        ERROR_HEATER_DERIVATIVE_GAIN_OUT_OF_RANGE,
        ERROR_HEATER_TIN_SELECT_OUT_OF_RANGE,
        ERROR_TIN,
    };
    enum HeaterTinSelect {
        HEATER_T1_SELECT,
        HEATER_T2_SELECT,
        HEATER_T3_SELECT,
        HEATER_T4_SELECT,
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
    void startTrace();
    void stopTrace();
    int setTraceResolution(int res);
    int getTraceResolution();
    int getTraceFirstSample();
    int getTraceNumberOfSamples();
    int setTraceNumberOfReadSamples(int number);
    void heaterClosedLoopEnable();
    void heaterClosedLoopDisable();
    int heaterSetProportionalGain(float gain);
    float heaterGetProportionalGain();
    int heaterSetIntegralGain(float gain);
    float heaterGetIntegrallGain();
    int heaterSetDerivativeGain(float gain);
    float heaterGetDerivativeGain();
    bool isHeaterClosedLoopEnabled();
    void heaterEnable(bool en);
    bool isHeaterEnabled();
    int setHeaterRefTemperature(float tref);
    float getHeaterRefTemperature();
    int setHeaterImax(float imax);
    float getHeaterImax();
    int heaterSetTin(int tin);
    int heaterGetTin();
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
    bool isRefTemperatureValid(float tref);
    bool isImaxValid(float imax);
    int getTin(float& tin);
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
    int m_snapshotSample;
    LibCircularBuffer m_traceCircularBuffer;
    static SemaphoreHandle_t s_traceMutex;
    bool m_isTraceRunning;
    int m_traceRes;
    bool m_isHeaterEnabled;
    bool m_isHeaterClosedLoopEnabled;
    bool m_isHeaterClosedLoopInitialized;
    float m_pidHeaterProportionalGain;
    float m_pidHeaterIntegralGain;
    float m_pidHeaterDerivativeGain;
    float m_heaterRefTemperature;
    float m_heaterImax;
    int m_heaterTin;
    float m_heaterPrevError;
    float m_heaterAccError;
};

#endif // _LIB_TEC_H_
