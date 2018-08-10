#pragma once

#include <FreeRTOS.h>
#include <os_task.h>
#include <sys_common.h>
#include <rti.h>
#include <sys_pmu.h>
#include <system.h>

namespace LibDelay
{
    inline void waitForTimer(uint32 timeInMicrosec)
    {
        uint32 ticks = timeInMicrosec * VCLK1_FREQ;
        _pmuEnableCountersGlobal_();
        _pmuStartCounters_(pmuCYCLE_COUNTER);
        const uint32 start = _pmuGetCycleCount_();
        while ((_pmuGetCycleCount_() - start) < ticks)
        {
        }
    }

    inline void waitForTimerInTheCriticalSection(uint32 timeInMicrosec)
    {
       uint32 ticks    = RTI_UDCP1_CONFIGVALUE * timeInMicrosec;
       uint32 counter  = rtiCOUNTER_BLOCK1;
       rtiREG1->GCTRL &= ~(uint32)((uint32)1U << (counter & 3U)); // Stop counter
       //
       // Clear counter
       //
       rtiREG1->CNT[counter].UCx   = 0x00000000U;
       rtiREG1->CNT[counter].FRCx  = 0x00000000U;
       rtiREG1->CMP[counter].COMPx = 0x00000000U;
       taskENTER_CRITICAL();
       rtiREG1->GCTRL |= ((uint32)1U << (counter & 3U)); // Start counter
       while (rtiREG1->CMP[rtiCOMPARE1].COMPx < ticks);
       taskEXIT_CRITICAL();
    }

    inline void startTimerAndEnterCriticalSection()
    {
       uint32 counter  = rtiCOUNTER_BLOCK1;
       rtiREG1->GCTRL &= ~(uint32)((uint32)1U << (counter & 3U)); // Stop counter
       //
       // Clear counter
       //
       rtiREG1->CNT[counter].UCx   = 0x00000000U;
       rtiREG1->CNT[counter].FRCx  = 0x00000000U;
       rtiREG1->CMP[counter].COMPx = 0x00000000U;;
       taskENTER_CRITICAL();
       rtiREG1->GCTRL |= ((uint32)1U << (counter & 3U)); // Start counter
    }

    inline void waitForTimerAndExitCriticalSection(uint32 timeInMicrosec)
    {
       uint32 ticks    = RTI_UDCP1_CONFIGVALUE * timeInMicrosec;
       while (rtiREG1->CMP[rtiCOMPARE1].COMPx < ticks);
       taskEXIT_CRITICAL();
    }

    inline void waitMicrosec(uint32 time)
    {
        uint32 ticks    = RTI_UDCP1_CONFIGVALUE * time;
        uint32 counter  = rtiCOUNTER_BLOCK1;
        rtiREG1->GCTRL &= ~(uint32)((uint32)1U << (counter & 3U)); // Stop counter
        //
        // Clear counter
        //
        rtiREG1->CNT[counter].UCx   = 0x00000000U;
        rtiREG1->CNT[counter].FRCx  = 0x00000000U;
        rtiREG1->CMP[counter].COMPx = 0x00000000U;
        rtiREG1->GCTRL |= ((uint32)1U << (counter & 3U)); // Start counter
        while (rtiREG1->CMP[rtiCOMPARE1].COMPx < ticks);
    }
}
