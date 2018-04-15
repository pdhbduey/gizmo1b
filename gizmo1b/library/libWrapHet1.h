#ifndef _LIB_WRAP_HET1_H_
#define _LIB_WRAP_HET1_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "reg_het.h"
#include "libWrapGioPort.h"
#include "libWrapHet.h"

class LibWrapHet1 : public LibWrapGioPort, public LibWrapHet
{
public:
    LibWrapHet1();
    virtual ~LibWrapHet1();
protected:
    virtual SemaphoreHandle_t& getPortMutex();
    virtual gioPORT_t* getPort();
    virtual hetRAMBASE_t* getHetRamBase();
    virtual SemaphoreHandle_t& getHetMutex();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_portMutex;
    static SemaphoreHandle_t s_hetMutex;
    gioPORT_t* m_port;
    hetRAMBASE_t* m_hetRamBase;
};

#endif // _LIB_WRAP_HET1_H_
