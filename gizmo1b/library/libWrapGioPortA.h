#ifndef _LIB_WRAP_GIO_PORTA_H_
#define _LIB_WRAP_GIO_PORTA_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "libWrapGioPort.h"

class LibWrapGioPortA : public LibWrapGioPort
{
public:
    LibWrapGioPortA();
    virtual ~LibWrapGioPortA();
private:
    virtual gioPORT_t* getPort();
    virtual SemaphoreHandle_t& getMutex();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    gioPORT_t* m_port;
};

#endif // _LIB_WRAP_GIO_PORTA_H_
