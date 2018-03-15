#ifndef _LIB_MUTEX_H_
#define _LIB_MUTEX_H_

#include "FreeRTOS.h"
#include "os_semphr.h"

class LibMutex
{
public:
    LibMutex(SemaphoreHandle_t& mutex, TickType_t delay = portMAX_DELAY);
    virtual ~LibMutex();
private:
    SemaphoreHandle_t& m_mutex;
    TickType_t m_delay;
};

#endif // _LIB_MUTEX_H_
