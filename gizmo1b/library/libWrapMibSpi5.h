#ifndef _LIB_WRAP_MIB_SPI5_H_
#define _LIB_WRAP_MIB_SPI5_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "libWrapGioPort.h"

class LibWrapMibSpi5 : public LibWrapGioPort
{
public:
    LibWrapMibSpi5();
    virtual ~LibWrapMibSpi5();
private:
    virtual gioPORT_t* getPort();
    virtual SemaphoreHandle_t& getMutex();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    gioPORT_t* m_port;
};

#endif // _LIB_WRAP_MIB_SPI5_H_
