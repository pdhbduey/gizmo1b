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

SemaphoreHandle_t LibWrapMibSpi1::s_mutex;
SemaphoreHandle_t LibWrapMibSpi1::s_spi1SomiSwMutex;

LibWrapMibSpi1::LibWrapMibSpi1() :
    m_port(mibspiPORT1),
    m_base(mibspiREG1)
{
    if (!s_mutex) {
        s_mutex = xSemaphoreCreateMutex();
        s_spi1SomiSwMutex = xSemaphoreCreateMutex();
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
}

void LibWrapMibSpi1::getData(uint32 group, uint16* data)
{
    LibMutex libMutex(s_mutex);
}

void LibWrapMibSpi1::transfer(uint32 group)
{
    LibMutex libMutex(s_mutex);
}

bool LibWrapMibSpi1::isTransferComplete(uint32 group)
{
    LibMutex libMutex(s_mutex);
    return false;
}
