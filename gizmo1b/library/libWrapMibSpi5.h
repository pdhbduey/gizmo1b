#ifndef _LIB_WRAP_MIB_SPI5_H_
#define _LIB_WRAP_MIB_SPI5_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "libWrapMibSpi.h"

class LibWrapMibSpi5 : public LibWrapMibSpi
{
public:
    LibWrapMibSpi5();
    virtual ~LibWrapMibSpi5();
    virtual void setBit(uint32 bit, uint32 value);
    virtual uint32 getBit(uint32 bit);
private:
    static SemaphoreHandle_t s_mutex;
    gioPORT_t* m_port;
};

#endif // _LIB_WRAP_MIB_SPI5_H_
