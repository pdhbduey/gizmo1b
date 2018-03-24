#include "sys_pmu.h"
#include "system.h"
#include "libDelay.h"

#define US_TO_CYCLES(us) (us * VCLK1_FREQ)

void LibDelay::us(uint32 usDelay)
{
    uint32 delay = US_TO_CYCLES(usDelay);
    _pmuEnableCountersGlobal_();
    _pmuStartCounters_(pmuCYCLE_COUNTER);
    const uint32 start = _pmuGetCycleCount_();
    while ((_pmuGetCycleCount_() - start) < delay)
    {
    }
}

