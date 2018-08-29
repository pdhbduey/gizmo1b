#include <mibspi.h>
#include <libMutex.h>
#include <LibMibSpi3.h>

// 53:MIBSPI3CLK:MCU_SPI3_CLK
// 51:MIBSPI3SOMI:MCU_SPI3_SOMI
// 52:MIBSPI3SIMO:MCU_SPI3_SIMO

SemaphoreHandle_t LibMibSpi3::s_sem;
SemaphoreHandle_t LibMibSpi3::s_mibSpiMutex;
bool LibMibSpi3::s_isInitialized;

LibMibSpi3::LibMibSpi3() :
    m_mibSpiBase(mibspiREG3)
{
    if (!s_isInitialized) {
        s_mibSpiMutex = xSemaphoreCreateMutex();
        s_sem = xSemaphoreCreateBinary();
        addNotification(this);
        s_isInitialized = true;
    }
}

mibspiBASE_t* LibMibSpi3::getMibSpiBase()
{
    return m_mibSpiBase;
}

SemaphoreHandle_t& LibMibSpi3::getMibSpiMutex()
{
    return s_mibSpiMutex;
}

SemaphoreHandle_t& LibMibSpi3::getSem()
{
    return s_sem;
}
