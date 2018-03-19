#include "libMutex.h"

LibMutex::LibMutex(SemaphoreHandle_t& mutex, int msTimeout) :
    m_mutex(mutex),
    m_msTimeout(msTimeout)
{
    xSemaphoreTake(m_mutex, pdMS_TO_TICKS(m_msTimeout));
}

LibMutex::~LibMutex()
{
    xSemaphoreGive(m_mutex);
}
