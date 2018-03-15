#include "mibspi.h"
#include "libWrapMibSpi.h"

bool LibWrapMibSpi::s_isInitialized;

LibWrapMibSpi::LibWrapMibSpi()
{
    if (!s_isInitialized) {
        mibspiInit();
        s_isInitialized = true;
    }
}

LibWrapMibSpi::~LibWrapMibSpi()
{
}
