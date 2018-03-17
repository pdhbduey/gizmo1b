#ifndef _LIB_WRAP_MIB_SPI3_H_
#define _LIB_WRAP_MIB_SPI3_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "reg_mibspi.h"
#include "libWrapMibSpi.h"
#include "libWrapGioPort.h"

class LibWrapMibSpi3 : public LibWrapMibSpi, public LibWrapGioPort
{
public:
    LibWrapMibSpi3();
    virtual ~LibWrapMibSpi3();
    // LibWrapGioPort interface
    virtual void setBit(uint32 bit, uint32 value);
    virtual uint32 getBit(uint32 bit);
    // LibWrapMibSpi3 interface
    virtual void setData(uint32 group, uint16* data);
    virtual void getData(uint32 group, uint16* data);
    virtual void transfer(uint32 group);
    virtual bool isTransferComplete(uint32 group);
private:
    static SemaphoreHandle_t s_mutex;
    gioPORT_t* m_port;
    mibspiBASE_t* m_base;
};

#endif // _LIB_WRAP_MIB_SPI3_H_
