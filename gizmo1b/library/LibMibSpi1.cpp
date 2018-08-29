#include <mibspi.h>
#include <libMutex.h>
#include <LibMibSpi1.h>

// 95:MIBSPI1CLK:MCU_SPI1_CLK
// 94:MIBSPI1SOMI[0]:MCU_SPI1_SOMI
// 93:MIBSPI1SIMO[0]:MCU_SPI1_SIMO

SemaphoreHandle_t LibMibSpi1::s_sem;
SemaphoreHandle_t LibMibSpi1::s_mibSpiMutex;
bool LibMibSpi1::s_isInitialized;

LibMibSpi1::LibMibSpi1() :
    m_mibSpiBase(mibspiREG1)
{
    if (!s_isInitialized) {
        s_mibSpiMutex = xSemaphoreCreateMutex();
        s_sem = xSemaphoreCreateBinary();
        addNotification(this);
        s_isInitialized = true;
    }
}

mibspiBASE_t* LibMibSpi1::getMibSpiBase()
{
    return m_mibSpiBase;
}

SemaphoreHandle_t& LibMibSpi1::getMibSpiMutex()
{
    return s_mibSpiMutex;
}

SemaphoreHandle_t& LibMibSpi1::getSem()
{
    return s_sem;
}
