#include "FreeRTOS.h"
#include "os_semphr.h"
#include "mibspi.h"
#include "gio.h"
#include "libMutex.h"
#include "libWrapMibSpi3.h"

// 53:MIBSPI3CLK:MCU_SPI3_CLK
// 94:MIBSPI3SOMI:MCU_SPI3_SOMI
// 93:MIBSPI3SIMO:MCU_SPI3_SIMO

SemaphoreHandle_t LibWrapMibSpi3::s_mutex;

LibWrapMibSpi3::LibWrapMibSpi3() :
    m_port(mibspiPORT3),
    m_base(mibspiREG3)
{
    if (!s_mutex) {
        s_mutex = xSemaphoreCreateMutex();
    }
}

LibWrapMibSpi3::~LibWrapMibSpi3()
{
}

void LibWrapMibSpi3::setBit(uint32 bit, uint32 value)
{
    LibMutex libMutex(s_mutex);
    gioSetBit(m_port, bit, value);
}

uint32 LibWrapMibSpi3::getBit(uint32 bit)
{
    LibMutex libMutex(s_mutex);
    return gioGetBit(m_port, bit);
}

void LibWrapMibSpi3::setData(uint32 group, uint16* data)
{
    LibMutex libMutex(s_mutex);
}

void LibWrapMibSpi3::getData(uint32 group, uint16* data)
{
    LibMutex libMutex(s_mutex);
}

void LibWrapMibSpi3::transfer(uint32 group)
{
    LibMutex libMutex(s_mutex);
}

bool LibWrapMibSpi3::isTransferComplete(uint32 group)
{
    LibMutex libMutex(s_mutex);
    return false;
}
