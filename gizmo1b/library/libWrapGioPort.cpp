#include "gio.h"
#include "libWrapGioPort.h"

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
