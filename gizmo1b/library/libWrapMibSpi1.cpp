#include "FreeRTOS.h"
#include "os_semphr.h"
#include "mibspi.h"
#include "gio.h"
#include "libMutex.h"
#include "libWrapMibSpi1.h"

SemaphoreHandle_t LibWrapMibSpi1::s_mutex;

LibWrapMibSpi1::LibWrapMibSpi1() :
    m_port(mibspiPORT1)
{
    if (!s_mutex) {
        s_mutex = xSemaphoreCreateMutex();
    }
}

LibWrapMibSpi1::~LibWrapMibSpi1()
{
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
