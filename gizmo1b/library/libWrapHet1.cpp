#include "libWrapHet1.h"
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "het.h"
#include "gio.h"
#include "libMutex.h"

SemaphoreHandle_t LibWrapHet1::s_mutex;

LibWrapHet1::LibWrapHet1() :
    m_port(hetPORT1)
{
    if (!s_mutex) {
        s_mutex = xSemaphoreCreateMutex();
        hetInit();
    }
}

LibWrapHet1::~LibWrapHet1()
{
}

void LibWrapHet1::setBit(uint32 bit, uint32 value)
{
    LibMutex libMutex(s_mutex);
    gioSetBit(m_port, bit, value);
}

uint32 LibWrapHet1::getBit(uint32 bit)
{
    LibMutex libMutex(s_mutex);
    return gioGetBit(m_port, bit);
}
