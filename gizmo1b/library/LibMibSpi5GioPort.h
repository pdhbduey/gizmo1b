#pragma once

#include <FreeRTOS.h>
#include <os_semphr.h>
#include <reg_gio.h>
#include <libWrapGioPort.h>

class LibMibSpi5GioPort : public LibWrapGioPort
{
public:
    LibMibSpi5GioPort();
    virtual ~LibMibSpi5GioPort();
private:
    virtual gioPORT_t* getPort();
    virtual SemaphoreHandle_t& getPortMutex();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_portMutex;
    gioPORT_t* m_port;
};
