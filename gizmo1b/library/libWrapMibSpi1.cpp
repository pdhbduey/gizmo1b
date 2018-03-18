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
    m_base(mibspiREG1),
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
    m_somiSwMap[SOMI_SW]->m_libWrapGioPort->setBit(m_somiSwMap[SOMI_SW]->m_pin, somi);
}

void LibWrapMibSpi1::setBit(uint32 bit, uint32 value)
{
    LibMutex libMutex(s_mutex);
    gioSetBit(m_port, bit, value);
}

uint32 LibWrapMibSpi1::getBit(uint32 bit)
{
    LibMutex libMutex(s_mutex);
    return gioGetBit(m_port, bit);
}

void LibWrapMibSpi1::setData(uint32 group, uint16* data)
{
    LibMutex libMutex(s_mutex);
    mibspiSetData(m_base, group, data);
}

void LibWrapMibSpi1::getData(uint32 group, uint16* data)
{
    LibMutex libMutex(s_mutex);
    mibspiGetData(m_base, group, data);
}

void LibWrapMibSpi1::transfer(uint32 group)
{
    LibMutex libMutex(s_mutex);
    if (m_isLoopBack) {
        mibspiEnableLoopback(m_base, Digital_Lbk);
    }
    mibspiEnableGroupNotification(m_base, group, 0);
    mibspiTransfer(m_base, group);
}

bool LibWrapMibSpi1::waitForTransferComplete(uint32 group, int msTimeout)
{
    if (xSemaphoreTake(s_sem, pdMS_TO_TICKS(msTimeout)) == pdFALSE) {
        LibMutex libMutex(s_mutex);
        mibspiDisableLoopback(m_base);
        return false;
    }
    return true;
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

    for (int i = 0; i < 10; i++) {
        txBuffer[i] = i;
        rxBuffer[i] = 0;
    }

    libWrapMibSpi.setData(0, txBuffer);
    libWrapMibSpi.transfer(0);
    if (libWrapMibSpi.waitForTransferComplete(0)) {
        libWrapMibSpi.getData(0, rxBuffer);
    }
    else {
        return false;
    }
    for (int i = 0; i < 10; i++) {
        if (rxBuffer[i] != txBuffer[i]) {
            return false;
        }
    }
    return true;
}
