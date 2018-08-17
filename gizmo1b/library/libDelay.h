#pragma once

#include <FreeRTOS.h>
#include <os_task.h>
#include <sys_common.h>
#include <rti.h>
#include <sys_pmu.h>
#include <system.h>

namespace LibDelay
{
    inline void pmuMicrosecDelay(uint32 timeInMicrosec)
    {
        uint32 ticks = timeInMicrosec * VCLK1_FREQ;
        _pmuEnableCountersGlobal_();
        _pmuStartCounters_(pmuCYCLE_COUNTER);
        const uint32 start = _pmuGetCycleCount_();
        while ((_pmuGetCycleCount_() - start) < ticks)
        {
        }
    }

    inline void rtiReset()
    {
       uint32 counter  = rtiCOUNTER_BLOCK1;
       rtiREG1->GCTRL &= ~(uint32)((uint32)1U << (counter & 3U)); // Stop counter
       //
       // Clear counter
       //
       rtiREG1->CNT[counter].UCx   = 0x00000000U;
       rtiREG1->CNT[counter].FRCx  = 0x00000000U;
       rtiREG1->CMP[counter].COMPx = 0x00000000U;;
       rtiREG1->GCTRL |= ((uint32)1U << (counter & 3U)); // Start counter
    }

    inline void rtiMicrosecDelay(uint32 delay)
    {
       uint32 ticks    = RTI_UDCP1_CONFIGVALUE * delay;
       while (rtiREG1->CMP[rtiCOMPARE1].COMPx < ticks);
    }
}
