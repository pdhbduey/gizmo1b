#ifndef _LIB_WRAP_MIB_SPI_H_
#define _LIB_WRAP_MIB_SPI_H_

#include "libWrapGioPort.h"

class LibWrapMibSpi : public LibWrapGioPort
{
public:
    LibWrapMibSpi();
    virtual ~LibWrapMibSpi();
private:
    static bool s_isInitialized;
};

#endif // _LIB_WRAP_MIB_SPI_H_
