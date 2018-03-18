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

void LibWrapGioPort::setPin(uint32 pin, uint32 value)
{
    LibMutex libMutex(getPortMutex());
    gioSetBit(getPort(), pin, value);
}

uint32 LibWrapGioPort::getPin(uint32 pin)
{
    LibMutex libMutex(getPortMutex());
    return gioGetBit(getPort(), pin);
}
