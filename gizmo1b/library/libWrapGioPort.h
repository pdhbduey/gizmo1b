#ifndef _LIB_WRAP_GIO_PORT_H_
#define _LIB_WRAP_GIO_PORT_H_

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
    virtual void setBit(uint32 bit, uint32 value) = 0;
    virtual uint32 getBit(uint32 bit) = 0;
};

#endif // _LIB_WRAP_GIO_PORTA_H_
