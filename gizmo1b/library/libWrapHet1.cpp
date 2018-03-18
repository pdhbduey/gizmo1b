#include "libWrapHet1.h"
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "het.h"
#include "libMutex.h"

SemaphoreHandle_t LibWrapHet1::s_mutex;
bool LibWrapHet1::s_isInitialized;

LibWrapHet1::LibWrapHet1() :
    m_port(hetPORT1)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        hetInit();
        s_isInitialized = true;
    }
}

LibWrapHet1::~LibWrapHet1()
{
}

gioPORT_t* LibWrapHet1::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibWrapHet1::getMutex()
{
    return s_mutex;
}
