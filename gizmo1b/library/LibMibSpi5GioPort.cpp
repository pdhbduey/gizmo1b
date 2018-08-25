#include <mibspi.h>
#include <gio.h>
#include <libMutex.h>
#include <LibMibSpi5GioPort.h>

SemaphoreHandle_t LibMibSpi5GioPort::s_portMutex;
bool LibMibSpi5GioPort::s_isInitialized;

LibMibSpi5GioPort::LibMibSpi5GioPort() :
    m_port(mibspiPORT5)
{
    if (!s_isInitialized) {
        s_portMutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibMibSpi5GioPort::~LibMibSpi5GioPort()
{
}

gioPORT_t* LibMibSpi5GioPort::getPort()
{
    return m_port;
}

SemaphoreHandle_t& LibMibSpi5GioPort::getPortMutex()
{
    return s_portMutex;
}
