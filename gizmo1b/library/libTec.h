#ifndef _LIB_TEC_H_
#define _LIB_TEC_H_

#include <vector>
#include <utility>
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "libWrapGioPort.h"
#include "libAdc.h"

class LibTec
{
public:
    typedef std::pair<int, float> TimeCurrent;
    enum Status {
       OKAY,
       ERROR_ISENSE,
       ERROR_VSENSE,
    };
public:
    LibTec();
    virtual ~LibTec();
    void enable(bool en);
    int getISense(float& iSense); // Calculated Current Based on ADC voltage
    int getVSense(float& vSense); // Calculated Voltage Based on ADC voltage
    // Calculated Closed Loop Current Output (Iref)
    void setCurrent(float value); // -15A..+15A
    // Linear Interpolation over time between set points,
    // Repeat output LOOPCOUNT times, Time (us)
    void setCurrent(int loopCount, std::vector<TimeCurrent>& setPoints);
    void stopCurrent();
    int getLoopValue();
    int getCountValue();
private:
    enum adcChannels {
        ISENSE = LibAdc::CHANNEL_1,
        VSENSE = LibAdc::CHANNEL_0,
    };
private:
    LibWrapGioPort::Port m_tecEnable;
    LibAdc m_libAdc;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
};

#endif // _LIB_TEC_H_
