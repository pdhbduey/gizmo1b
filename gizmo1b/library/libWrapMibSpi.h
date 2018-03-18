#ifndef _LIB_WRAP_MIB_SPI_H_
#define _LIB_WRAP_MIB_SPI_H_

#include <map>
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "sys_common.h"
#include "mibspi.h"


class LibWrapMibSpi
{
public:
    enum Group {
        GROUP_0,
        GROUP_1,
        GROUP_2,
        GROUP_3,
        GROUP_4,
        GROUP_5,
        GROUP_6,
        GROUP_7,
    };
public:
    LibWrapMibSpi();
    virtual ~LibWrapMibSpi();
    void setData(uint32 group, uint16* data);
    void getData(uint32 group, uint16* data);
    void transfer(uint32 group);
    bool waitForTransferComplete(uint32 group, int msTimeout = 1000);
    friend void mibspiGroupNotification(mibspiBASE_t* mibspiReg, uint32 group);
protected:
    virtual SemaphoreHandle_t& getMutex() = 0;
    virtual mibspiBASE_t* getMibSpiBase() = 0;
    virtual bool isLoopBack() = 0;
    virtual SemaphoreHandle_t& getSem() = 0;
    void addNotification(mibspiBASE_t* mibspiReg, void (*notification)(uint32));
private:
    static bool s_isInitialized;
    std::map<mibspiBASE_t*, void (*)(uint32)> m_notificationMap;
    static std::map<mibspiBASE_t*, void (*)(uint32)>* s_notificationMap;
};

#endif // _LIB_WRAP_MIB_SPI_H_
