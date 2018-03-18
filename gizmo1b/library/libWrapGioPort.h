#ifndef _LIB_WRAP_GIO_PORT_H_
#define _LIB_WRAP_GIO_PORT_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "sys_common.h"

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
    void setBit(uint32 bit, uint32 value);
    uint32 getBit(uint32 bit);
protected:
    virtual SemaphoreHandle_t& getMutex() = 0;
    virtual gioPORT_t* getPort() = 0;
private:
    static bool s_isInitialized;
};

#endif // _LIB_WRAP_GIO_PORTA_H_
