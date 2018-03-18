#include "FreeRTOS.h"
#include "os_semphr.h"
#include "mibspi.h"
#include "gio.h"
#include "libMutex.h"
#include "libWrapMibSpi5.h"

SemaphoreHandle_t LibWrapMibSpi5::s_mutex;
bool LibWrapMibSpi5::s_isInitialized;

LibWrapMibSpi5::LibWrapMibSpi5() :
    m_port(mibspiPORT5)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibWrapMibSpi5::~LibWrapMibSpi5()
{
}

gioPORT_t* LibWrapMibSpi5::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibWrapMibSpi5::getMutex()
{
    return s_mutex;
}
