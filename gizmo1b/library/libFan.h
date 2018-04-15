#ifndef _LIB_FAN_H_
#define _LIB_FAN_H_

#include "sys_common.h"
#include "libWrapHet1.h"

class LibFan
{
public:
    LibFan();
    virtual ~LibFan();
    int setPwm1DutyCycle(uint32 dutyCycle);   // 0%-100%
    int setPwm2DutyCycle(uint32 dutyCycle);   // 0%-100%
    uint32 getPwm1DutyCycle();
    uint32 getPwm2DutyCycle();
    int setPwm1PeriodInUs(float periodInUs); // 10us-1,000,000us
    int setPwm2PeriodInUs(float periodInUs); // 10us-1,000,000us
    float getPwm1PeriodInUs();
    float getPwm2PeriodInUs();
    float getSensor1Rpm();
    float getSensor2Rpm();
private:
    float getRpmFromPerid(float periodInUs);
private:
    LibWrapHet1 m_libWrapHet1;
};

#endif // _LIB_FAN_H_
