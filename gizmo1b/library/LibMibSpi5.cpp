#include <mibspi.h>
#include <libMutex.h>
#include <LibMibSpi5.h>

// 100:MIBSPI5CLK :T_ADC_SPI_CLK
//  98:MIBSPI5SOMI:T_ADC_SPI_SOMI
//  99:MIBSPI5SIMO:T_ADC_SPI_SIMO

SemaphoreHandle_t LibMibSpi5::s_sem;
SemaphoreHandle_t LibMibSpi5::s_mibSpiMutex;
bool LibMibSpi5::s_isInitialized;

LibMibSpi5::LibMibSpi5() :
    m_mibSpiBase(mibspiREG5)
{
    if (!s_isInitialized) {
        s_mibSpiMutex = xSemaphoreCreateMutex();
        s_sem = xSemaphoreCreateBinary();
        addNotification(this);
        s_isInitialized = true;
    }
}

mibspiBASE_t* LibMibSpi5::getMibSpiBase()
{
    return m_mibSpiBase;
}

SemaphoreHandle_t& LibMibSpi5::getMibSpiMutex()
{
    return s_mibSpiMutex;
}

SemaphoreHandle_t& LibMibSpi5::getSem()
{
    return s_sem;
}
