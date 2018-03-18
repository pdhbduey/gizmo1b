#include "libWrapGioPortA.h"
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "gio.h"
#include "libMutex.h"

SemaphoreHandle_t LibWrapGioPortA::s_portMutex;
bool LibWrapGioPortA::s_isInitialized;

LibWrapGioPortA::LibWrapGioPortA() :
    m_port(gioPORTA)
{
    if (!s_isInitialized) {
        s_portMutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibWrapGioPortA::~LibWrapGioPortA()
{
}

gioPORT_t* LibWrapGioPortA::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibWrapGioPortA::getPortMutex()
{
    return s_portMutex;
}

