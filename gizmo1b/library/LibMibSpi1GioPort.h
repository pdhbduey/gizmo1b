#pragma once

#include <FreeRTOS.h>
#include <os_semphr.h>
#include <reg_gio.h>
#include <libWrapGioPort.h>

class LibMibSpi1GioPort : public LibWrapGioPort
{
public:
    LibMibSpi1GioPort();
    virtual ~LibMibSpi1GioPort();
private:
    virtual gioPORT_t* getPort();
    virtual SemaphoreHandle_t& getPortMutex();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_portMutex;
    gioPORT_t* m_port;
};
