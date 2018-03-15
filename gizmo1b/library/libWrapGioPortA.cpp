#include "libWrapGioPortA.h"
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "gio.h"
#include "libMutex.h"

SemaphoreHandle_t LibWrapGioPortA::s_mutex;

LibWrapGioPortA::LibWrapGioPortA() :
    m_port(gioPORTA)
{
    if (!s_mutex) {
        s_mutex = xSemaphoreCreateMutex();
        gioInit();
    }
}

LibWrapGioPortA::~LibWrapGioPortA()
{
}

void LibWrapGioPortA::setBit(uint32 bit, uint32 value)
{
    LibMutex libMutex(s_mutex);
    gioSetBit(m_port, bit, value);
}

uint32 LibWrapGioPortA::getBit(uint32 bit)
{
    LibMutex libMutex(s_mutex);
    return gioGetBit(m_port, bit);
}
