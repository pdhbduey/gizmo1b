#ifndef _LIB_WRAP_MIB_SPI3_H_
#define _LIB_WRAP_MIB_SPI3_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "libWrapGioPort.h"

class LibWrapMibSpi3 : public LibWrapGioPort
{
public:
    LibWrapMibSpi3();
    virtual ~LibWrapMibSpi3();
private:
    virtual gioPORT_t* getPort();
    virtual SemaphoreHandle_t& getPortMutex();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_portMutex;
    gioPORT_t* m_port;
};

#endif // _LIB_WRAP_MIB_SPI3_H_
