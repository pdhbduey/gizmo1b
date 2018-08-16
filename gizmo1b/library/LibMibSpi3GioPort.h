#pragma once

#include <FreeRTOS.h>
#include <os_semphr.h>
#include <reg_gio.h>
#include <reg_mibspi.h>
#include <libWrapMibSpi.h>
#include <libWrapGioPort.h>

class LibMibSpi3GioPort : public LibWrapGioPort
{
public:
    LibMibSpi3GioPort();
    virtual ~LibMibSpi3GioPort();
private:
    virtual gioPORT_t* getPort();
    virtual SemaphoreHandle_t& getPortMutex();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_portMutex;
    gioPORT_t* m_port;
};
