#include "libMutex.h"

LibMutex::LibMutex(SemaphoreHandle_t& mutex, TickType_t delay) :
    m_mutex(mutex),
    m_delay(delay)
{
    xSemaphoreTake(m_mutex, m_delay);
}

LibMutex::~LibMutex()
{
    xSemaphoreGive(m_mutex);
}
