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
SemaphoreHandle_t LibWrapMibSpi1::s_mutex;
SemaphoreHandle_t LibWrapMibSpi1::s_spi1SomiSwMutex;
bool LibWrapMibSpi1::s_isInitialized;

LibWrapMibSpi1::LibWrapMibSpi1(bool isLoopBack) :
    m_port(mibspiPORT1),
    m_mibSpiBase(mibspiREG1),
    m_isLoopBack(isLoopBack)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_spi1SomiSwMutex = xSemaphoreCreateMutex();
        addNotification(mibspiREG1, notification);
        s_sem = xSemaphoreCreateBinary();
        s_isInitialized = true;
    }
    m_somiSwMap[SOMI_SW] = new LibWrapGioPort::Port(new LibWrapMibSpi3, PIN_ENA); // 54:MIBSPI3NENA:MCU_SPI1_SOMI_SW
}

LibWrapMibSpi1::~LibWrapMibSpi1()
{
}

void LibWrapMibSpi1::somiSelect(int somi)
{
    m_somiSwMap[SOMI_SW]->m_libWrapGioPort->setPin(m_somiSwMap[SOMI_SW]->m_pin, somi);
}

gioPORT_t* LibWrapMibSpi1::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibWrapMibSpi1::getMutex()
{
    return s_mutex;
}

mibspiBASE_t* LibWrapMibSpi1::getMibSpiBase()
{
    return m_mibSpiBase;
}

bool LibWrapMibSpi1::isLoopBack()
{
    return m_isLoopBack;
}

SemaphoreHandle_t& LibWrapMibSpi1::getSem()
{
    return s_sem;
}

void LibWrapMibSpi1::notification(uint32 group)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(s_sem, &xHigherPriorityTaskWoken);
}

bool LibWrapMibSpi1::test()
{
    LibWrapMibSpi& libWrapMibSpi = *new LibWrapMibSpi1(true);
    uint16 txBuffer[10];
    uint16 rxBuffer[10];
    bool isPass = true;
    for (int i = 0; i < 10; i++) {
        txBuffer[i] = i;
        rxBuffer[i] = 0;
    }
    libWrapMibSpi.lock();
    libWrapMibSpi.setData(0, txBuffer);
    libWrapMibSpi.transfer(0);
    if (libWrapMibSpi.waitForTransferComplete(0)) {
        libWrapMibSpi.getData(0, rxBuffer);
    }
    else {
        isPass = false;
    }
    for (int i = 0; i < 10; i++) {
        if (rxBuffer[i] != txBuffer[i]) {
            isPass = false;
            break;
        }
    }
    libWrapMibSpi.unlock();
    return isPass;
}
