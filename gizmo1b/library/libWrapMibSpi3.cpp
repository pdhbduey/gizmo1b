#include "FreeRTOS.h"
#include "os_semphr.h"
#include "mibspi.h"
#include "gio.h"
#include "libMutex.h"
#include "libWrapMibSpi3.h"

// 53:MIBSPI3CLK:MCU_SPI3_CLK
// 94:MIBSPI3SOMI:MCU_SPI3_SOMI
// 93:MIBSPI3SIMO:MCU_SPI3_SIMO

SemaphoreHandle_t LibWrapMibSpi3::s_portMutex;
bool LibWrapMibSpi3::s_isInitialized;

LibWrapMibSpi3::LibWrapMibSpi3() :
    m_port(mibspiPORT3)
{
    if (!s_isInitialized) {
        s_portMutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibWrapMibSpi3::~LibWrapMibSpi3()
{
}

gioPORT_t* LibWrapMibSpi3::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibWrapMibSpi3::getPortMutex()
{
    return s_portMutex;
}
