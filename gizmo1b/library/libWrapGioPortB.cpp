#include "libWrapGioPortB.h"
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "gio.h"
#include "libMutex.h"

SemaphoreHandle_t LibWrapGioPortB::s_mutex;
bool LibWrapGioPortB::s_isInitialized;

LibWrapGioPortB::LibWrapGioPortB() :
    m_port(gioPORTB)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibWrapGioPortB::~LibWrapGioPortB()
{
}

gioPORT_t* LibWrapGioPortB::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibWrapGioPortB::getMutex()
{
    return s_mutex;
}

