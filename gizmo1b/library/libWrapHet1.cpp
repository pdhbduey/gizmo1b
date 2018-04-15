#include "libWrapHet1.h"
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "het.h"
#include "libMutex.h"

SemaphoreHandle_t LibWrapHet1::s_portMutex;
SemaphoreHandle_t LibWrapHet1::s_hetMutex;
bool LibWrapHet1::s_isInitialized;

LibWrapHet1::LibWrapHet1() :
    m_port(hetPORT1),
    m_hetRamBase(hetRAM1)
{
    if (!s_isInitialized) {
        s_portMutex = xSemaphoreCreateMutex();
        s_hetMutex = xSemaphoreCreateMutex();
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

SemaphoreHandle_t& LibWrapHet1::getPortMutex()
{
    return s_portMutex;
}

hetRAMBASE_t* LibWrapHet1::getHetRamBase()
{
    return m_hetRamBase;
}

SemaphoreHandle_t& LibWrapHet1::getHetMutex()
{
    return s_hetMutex;
}
