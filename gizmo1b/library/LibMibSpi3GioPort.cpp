
#include <mibspi.h>
#include <gio.h>
#include <libMutex.h>
#include <LibMibSpi3GioPort.h>

SemaphoreHandle_t LibMibSpi3GioPort::s_portMutex;
bool LibMibSpi3GioPort::s_isInitialized;

LibMibSpi3GioPort::LibMibSpi3GioPort() :
    m_port(mibspiPORT3)
{
    if (!s_isInitialized) {
        s_portMutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibMibSpi3GioPort::~LibMibSpi3GioPort()
{
}

gioPORT_t* LibMibSpi3GioPort::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibMibSpi3GioPort::getPortMutex()
{
    return s_portMutex;
}
