#include "het.h"
#include "libMutex.h"
#include "LibWrapHet.h"

bool LibWrapHet::s_isInitialized;

LibWrapHet::LibWrapHet()
{
    if (!s_isInitialized) {
        hetInit();
        s_isInitialized = true;
    }
}

LibWrapHet::~LibWrapHet()
{
}

int LibWrapHet::setPwmDutyCycle(uint32 pwm, uint32 dutyCycle)
{
    if (dutyCycle > 100) {
        return ERROR_DUTY_CYCLE_OUT_OF_RANGE;
    }
    LibMutex libMutex(getHetMutex());
    pwmSetDuty(getHetRamBase(), pwm, dutyCycle);
    return OKAY;
}

uint32 LibWrapHet::getPwmDutyCycle(uint32 pwm)
{
    LibMutex libMutex(getHetMutex());
    hetSIGNAL_t signal;
    pwmGetSignal(getHetRamBase(), pwm, &signal);
    return signal.duty;
}

int LibWrapHet::setPwmPeriodInUs(uint32 pwm, float periodInUs)
{
    if (periodInUs < 10 || periodInUs > 1000000) {
        return ERROR_PERIOD_IN_US_OUT_OF_RANGE;
    }
    hetSIGNAL_t signal;
    pwmGetSignal(getHetRamBase(), pwm, &signal);
    signal.period = periodInUs;
    pwmSetSignal(getHetRamBase(), pwm, signal);
    return OKAY;
}

float LibWrapHet::getPwmPeriodInUs(uint32 pwm)
{
    LibMutex libMutex(getHetMutex());
    hetSIGNAL_t signal;
    pwmGetSignal(getHetRamBase(), pwm, &signal);
    return signal.period;
}

float LibWrapHet::getCapPeriodInUs(uint32 cap)
{
    LibMutex libMutex(getHetMutex());
    hetSIGNAL_t signal;
    capGetSignal(getHetRamBase(), cap, &signal);
    return signal.period;
}
