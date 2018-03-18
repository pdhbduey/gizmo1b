#include "mibspi.h"
#include "libWrapMibSpi.h"

bool LibWrapMibSpi::s_isInitialized;
std::map<mibspiBASE_t*, void (*)(uint32)>* LibWrapMibSpi::s_notificationMap;

LibWrapMibSpi::LibWrapMibSpi()
{
    if (!s_isInitialized) {
        mibspiInit();
        s_notificationMap = &m_notificationMap;
        s_isInitialized = true;
    }
}

LibWrapMibSpi::~LibWrapMibSpi()
{
}

void LibWrapMibSpi::addNotification(mibspiBASE_t* mibspiReg, void (*notification)(uint32))
{
    m_notificationMap[mibspiReg] = notification;
}

extern "C" void mibspiGroupNotification(mibspiBASE_t* mibspiReg, uint32 group)
{
    if (LibWrapMibSpi::s_notificationMap->find(mibspiReg) != LibWrapMibSpi::s_notificationMap->end()
   && (*LibWrapMibSpi::s_notificationMap)[mibspiReg]) {
        mibspiDisableGroupNotification(mibspiReg, group);
        (*LibWrapMibSpi::s_notificationMap)[mibspiReg](group);
    }
}
