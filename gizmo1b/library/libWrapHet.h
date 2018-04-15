#ifndef _LIB_WRAP_HET_H_
#define _LIB_WRAP_HET_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "het.h"

class LibWrapHet
{
public:
    enum PWM {
        PWM_0 = pwm0,
        PWM_1 = pwm1,
    };
    enum CAP {
        CAP_0 = cap0,
        CAP_1 = cap1,
    };
    enum Status {
        OKAY,
        ERROR_DUTY_CYCLE_OUT_OF_RANGE,
        ERROR_PERIOD_IN_US_OUT_OF_RANGE,
    };
public:
    LibWrapHet();
    virtual ~LibWrapHet();
    int setPwmDutyCycle(uint32 pwm, uint32 dutyCycle); // 0%-100%
    uint32 getPwmDutyCycle(uint32 pwm);
    int setPwmPeriodInUs(uint32 pwm, float periodInUs); // 10us-1,000,000us
    float getPwmPeriodInUs(uint32 pwm);
    float getCapPeriodInUs(uint32 cap);
protected:
    virtual hetRAMBASE_t* getHetRamBase() = 0;
    virtual SemaphoreHandle_t& getHetMutex() = 0;
private:
    static bool s_isInitialized;
};

#endif // _LIB_WRAP_HET_H_
