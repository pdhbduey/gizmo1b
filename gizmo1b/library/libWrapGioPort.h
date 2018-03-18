#ifndef _LIB_WRAP_GIO_PORT_H_
#define _LIB_WRAP_GIO_PORT_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "sys_common.h"
#include "reg_gio.h"

class LibWrapGioPort
{
public:
    class Port {
    public:
        Port(LibWrapGioPort* libWrapGioPort, int pin);
        LibWrapGioPort* m_libWrapGioPort;
        int m_pin;
    };
public:
    LibWrapGioPort();
    virtual ~LibWrapGioPort();
    void setPin(uint32 pin, uint32 value);
    uint32 getPin(uint32 pin);
protected:
    virtual SemaphoreHandle_t& getMutex() = 0;
    virtual gioPORT_t* getPort() = 0;
private:
    static bool s_isInitialized;
};

#endif // _LIB_WRAP_GIO_PORTA_H_
