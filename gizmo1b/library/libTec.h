#ifndef _LIB_TEC_H_
#define _LIB_TEC_H_

#include <vector>
#include <utility>
#include "libWrapGioPort.h"

class LibTec
{
public:
    typedef std::pair<int, float> TimeCurrent;
public:
    LibTec();
    virtual ~LibTec();
    void enable(bool en);
    float getISense(); // Calculated Current Based on ADC voltage
    float getVSense(); // Calculated Voltage Based on ADC voltage
    // Calculated Closed Loop Current Output (Iref)
    void setCurrent(float value); // -15A..+15A
    // Linear Interpolation over time between set points,
    // Repeat output LOOPCOUNT times, Time (us)
    void setCurrent(int loopCount, std::vector<TimeCurrent>& setPoints);
    void stopCurrent();
    int getLoopValue();
    int getCountValue();
private:
    LibWrapGioPort::Port m_tecEnable;
};

#endif // _LIB_TEC_H_
