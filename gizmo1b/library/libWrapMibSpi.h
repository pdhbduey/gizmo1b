#ifndef _LIB_WRAP_MIB_SPI_H_
#define _LIB_WRAP_MIB_SPI_H_

#include "sys_common.h"


class LibWrapMibSpi
{
public:
    LibWrapMibSpi();
    virtual ~LibWrapMibSpi();
    virtual void setData(uint32 group, uint16* data) = 0;
    virtual void getData(uint32 group, uint16* data) = 0;
    virtual void transfer(uint32 group) = 0;
    virtual bool isTransferComplete(uint32 group) = 0;

private:
    static bool s_isInitialized;
};

#endif // _LIB_WRAP_MIB_SPI_H_
