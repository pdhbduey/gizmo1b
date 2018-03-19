#include "libWrapGioPortB.h"
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "gio.h"
#include "libMutex.h"

SemaphoreHandle_t LibWrapGioPortB::s_portMutex;
bool LibWrapGioPortB::s_isInitialized;

LibWrapGioPortB::LibWrapGioPortB() :
    m_port(gioPORTB)
{
    if (!s_isInitialized) {
        s_portMutex = xSemaphoreCreateMutex();
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

SemaphoreHandle_t& LibWrapGioPortB::getPortMutex()
{
    return s_portMutex;
}

