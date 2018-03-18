#ifndef _LIB_WRAP_MIB_SPI3_H_
#define _LIB_WRAP_MIB_SPI3_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "reg_mibspi.h"
#include "libWrapGioPort.h"

class LibWrapMibSpi3 : public LibWrapGioPort
{
public:
    LibWrapMibSpi3();
    virtual ~LibWrapMibSpi3();
    // LibWrapGioPort interface
    virtual void setBit(uint32 bit, uint32 value);
    virtual uint32 getBit(uint32 bit);
private:
    static SemaphoreHandle_t s_mutex;
    gioPORT_t* m_port;
};

#endif // _LIB_WRAP_MIB_SPI3_H_
