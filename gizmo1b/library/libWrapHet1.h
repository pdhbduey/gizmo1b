#ifndef _LIB_WRAP_HET1_H_
#define _LIB_WRAP_HET1_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "libWrapGioPort.h"

class LibWrapHet1 : public LibWrapGioPort
{
public:
    LibWrapHet1();
    virtual ~LibWrapHet1();
    // LibWrapGioPort interface
    virtual void setBit(uint32 bit, uint32 value);
    virtual uint32 getBit(uint32 bit);
private:
    static SemaphoreHandle_t s_mutex;
    gioPORT_t* m_port;
};

#endif // _LIB_WRAP_HET1_H_
