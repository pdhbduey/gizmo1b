#ifndef _LIB_WRAP_MIB_SPI1_H_
#define _LIB_WRAP_MIB_SPI1_H_

#include <map>
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "reg_mibspi.h"
#include "libWrapMibSpi.h"
#include "libWrapGioPort.h"

class LibWrapMibSpi1 : public LibWrapMibSpi, public LibWrapGioPort
{
public:
    enum SomiSelect {
        SPI_A,
        SPI_B,
    };
public:
    LibWrapMibSpi1();
    virtual ~LibWrapMibSpi1();
    void somiSelect(int somi);
    static bool test();
private:
    virtual gioPORT_t* getPort();
    virtual SemaphoreHandle_t& getPortMutex();
    virtual mibspiBASE_t* getMibSpiBase();
    virtual SemaphoreHandle_t& getMibSpiMutex();
    virtual SemaphoreHandle_t& getSem();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_portMutex;
    static SemaphoreHandle_t s_mibSpiMutex;
    gioPORT_t* m_port;
    mibspiBASE_t* m_mibSpiBase;
    static SemaphoreHandle_t s_spi1SomiSwMutex;
    LibWrapGioPort::Port m_somiSw;
    static SemaphoreHandle_t s_sem;
    uint32 m_group;
};

#endif // _LIB_WRAP_MIB_SPI1_H_
