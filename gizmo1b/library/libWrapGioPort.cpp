#include "gio.h"
#include "libWrapGioPort.h"
#include "libMutex.h"

bool LibWrapGioPort::s_isInitialized;

LibWrapGioPort::LibWrapGioPort()
{
    if (!s_isInitialized) {
        gioInit();
        s_isInitialized = true;
    }
}

LibWrapGioPort::~LibWrapGioPort()
{
}

LibWrapGioPort::Port::Port(LibWrapGioPort* libWrapGioPort, int pin) :
    m_libWrapGioPort(libWrapGioPort),
    m_pin(pin)
{
}

void LibWrapGioPort::setBit(uint32 bit, uint32 value)
{
    LibMutex libMutex(getMutex());
    gioSetBit(getPort(), bit, value);
}

uint32 LibWrapGioPort::getBit(uint32 bit)
{
    LibMutex libMutex(getMutex());
    return gioGetBit(getPort(), bit);
}
