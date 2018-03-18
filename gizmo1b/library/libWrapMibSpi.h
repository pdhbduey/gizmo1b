#ifndef _LIB_WRAP_MIB_SPI_H_
#define _LIB_WRAP_MIB_SPI_H_

#include <map>
#include "sys_common.h"
#include "reg_mibspi.h"


class LibWrapMibSpi
{
public:
    LibWrapMibSpi();
    virtual ~LibWrapMibSpi();
    virtual void setData(uint32 group, uint16* data) = 0;
    virtual void getData(uint32 group, uint16* data) = 0;
    virtual void transfer(uint32 group) = 0;
    virtual bool waitForTransferComplete(uint32 group, int msTimeout = 1000) = 0;
    friend void mibspiGroupNotification(mibspiBASE_t* mibspiReg, uint32 group);
protected:
    void addNotification(mibspiBASE_t* mibspiReg, void (*notification)(uint32));
private:
    static bool s_isInitialized;
    std::map<mibspiBASE_t*, void (*)(uint32)> m_notificationMap;
    static std::map<mibspiBASE_t*, void (*)(uint32)>* s_notificationMap;
};

#endif // _LIB_WRAP_MIB_SPI_H_
