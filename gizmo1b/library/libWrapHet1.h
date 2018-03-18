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
protected:
    virtual SemaphoreHandle_t& getPortMutex();
    virtual gioPORT_t* getPort();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_portMutex;
    gioPORT_t* m_port;
};

#endif // _LIB_WRAP_HET1_H_
