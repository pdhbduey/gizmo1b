#ifndef _LIB_WRAP_MIB_SPI1_H_
#define _LIB_WRAP_MIB_SPI1_H_

#include <map>
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "reg_gio.h"
#include "reg_mibspi.h"
#include "libWrapMibSpi.h"
#include "libWrapGioPort.h"
#include "libWrapMibSpi3.h"

class LibWrapMibSpi1 : public LibWrapMibSpi, public LibWrapGioPort
{
public:
    enum SomiSelect {
        SPI_A,
        SPI_B,
    };
    enum SomiSw {
        SOMI_SW
    };
    LibWrapMibSpi1(bool isLoopBack = false);
    virtual ~LibWrapMibSpi1();
    void somiSelect(int somi);
    // LibWrapGioPort interface
    virtual void setBit(uint32 bit, uint32 value);
    virtual uint32 getBit(uint32 bit);
    // LibWrapMibSpi interface
    virtual void setData(uint32 group, uint16* data);
    virtual void getData(uint32 group, uint16* data);
    virtual void transfer(uint32 group);
    virtual bool waitForTransferComplete(uint32 group, int msTimeout);
    static bool test();
private:
    static void notification(uint32 flags);
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    gioPORT_t* m_port;
    mibspiBASE_t* m_base;
    static SemaphoreHandle_t s_spi1SomiSwMutex;
    std::map<int, LibWrapGioPort::Port*> m_somiSwMap;
    static SemaphoreHandle_t s_sem;
    bool m_isLoopBack;
    uint32 m_group;
};

#endif // _LIB_WRAP_MIB_SPI1_H_
