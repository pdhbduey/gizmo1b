#include "FreeRTOS.h"
#include "os_semphr.h"
#include "mibspi.h"
#include "gio.h"
#include "libMutex.h"
#include "libWrapMibSpi1.h"
#include "libWrapMibSpi3.h"
#include "mibspi.h"

// 95:MIBSPI1CLK:MCU_SPI1_CLK
// 94:MIBSPI1SOMI[0]:MCU_SPI1_SOMI
// 93:MIBSPI1SIMO[0]:MCU_SPI1_SIMO

SemaphoreHandle_t LibWrapMibSpi1::s_sem;
SemaphoreHandle_t LibWrapMibSpi1::s_portMutex;
SemaphoreHandle_t LibWrapMibSpi1::s_mibSpiMutex;
SemaphoreHandle_t LibWrapMibSpi1::s_spi1SomiSwMutex;
bool LibWrapMibSpi1::s_isInitialized;

LibWrapMibSpi1::LibWrapMibSpi1() :
    m_port(mibspiPORT1),
    m_mibSpiBase(mibspiREG1),
    m_somiSw(new LibWrapMibSpi3, PIN_ENA) // 54:MIBSPI3NENA:MCU_SPI1_SOMI_SW
{
    if (!s_isInitialized) {
        s_portMutex = xSemaphoreCreateMutex();
        s_mibSpiMutex = xSemaphoreCreateMutex();
        s_spi1SomiSwMutex = xSemaphoreCreateMutex();
        s_sem = xSemaphoreCreateBinary();
        addNotification(this);
        s_isInitialized = true;
    }
}

LibWrapMibSpi1::~LibWrapMibSpi1()
{
}

void LibWrapMibSpi1::somiSelect(int somi)
{
    m_somiSw.m_libWrapGioPort->setPin(m_somiSw.m_pin, somi);
}

gioPORT_t* LibWrapMibSpi1::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibWrapMibSpi1::getPortMutex()
{
    return s_portMutex;
}

mibspiBASE_t* LibWrapMibSpi1::getMibSpiBase()
{
    return m_mibSpiBase;
}

SemaphoreHandle_t& LibWrapMibSpi1::getMibSpiMutex()
{
    return s_mibSpiMutex;
}

SemaphoreHandle_t& LibWrapMibSpi1::getSem()
{
    return s_sem;
}
