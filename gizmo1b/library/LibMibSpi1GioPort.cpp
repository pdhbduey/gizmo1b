#include <mibspi.h>
#include <gio.h>
#include <libMutex.h>
#include <LibMibSpi1GioPort.h>

SemaphoreHandle_t LibMibSpi1GioPort::s_portMutex;
bool LibMibSpi1GioPort::s_isInitialized;

LibMibSpi1GioPort::LibMibSpi1GioPort() :
    m_port(mibspiPORT1)
{
    if (!s_isInitialized) {
        s_portMutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibMibSpi1GioPort::~LibMibSpi1GioPort()
{
}

gioPORT_t* LibMibSpi1GioPort::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibMibSpi1GioPort::getPortMutex()
{
    return s_portMutex;
}
