#include "FreeRTOS.h"
#include "os_semphr.h"
#include "mibspi.h"
#include "gio.h"
#include "libMutex.h"
#include "libWrapMibSpi5.h"

SemaphoreHandle_t LibWrapMibSpi5::s_mutex;

LibWrapMibSpi5::LibWrapMibSpi5() :
    m_port(mibspiPORT5)
{
    if (!s_mutex) {
        s_mutex = xSemaphoreCreateMutex();
    }
}

LibWrapMibSpi5::~LibWrapMibSpi5()
{
}

void LibWrapMibSpi5::setBit(uint32 bit, uint32 value)
{
    LibMutex libMutex(s_mutex);
    gioSetBit(m_port, bit, value);
}

uint32 LibWrapMibSpi5::getBit(uint32 bit)
{
    LibMutex libMutex(s_mutex);
    return gioGetBit(m_port, bit);
}
