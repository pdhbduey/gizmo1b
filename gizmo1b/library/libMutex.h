#ifndef _LIB_MUTEX_H_
#define _LIB_MUTEX_H_

#include "FreeRTOS.h"
#include "os_semphr.h"

class LibMutex
{
public:
    LibMutex(SemaphoreHandle_t& mutex, int msTimeout = 1000);
    virtual ~LibMutex();
private:
    SemaphoreHandle_t& m_mutex;
    int m_msTimeout;
};

#endif // _LIB_MUTEX_H_
