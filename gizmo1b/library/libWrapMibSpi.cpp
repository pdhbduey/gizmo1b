#include "mibspi.h"
#include "libWrapMibSpi.h"

bool LibWrapMibSpi::s_isInitialized;
std::map<mibspiBASE_t*, LibWrapMibSpi*>* LibWrapMibSpi::s_notificationMap;

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

void LibWrapMibSpi::addNotification(LibWrapMibSpi* libWrapMibSpi)
{
    m_notificationMap[libWrapMibSpi->getMibSpiBase()] = libWrapMibSpi;
}

void LibWrapMibSpi::setData(uint32 group, uint16* data)
{
    mibspiSetData(getMibSpiBase(), group, data);
}

void LibWrapMibSpi::getData(uint32 group, uint16* data)
{
    mibspiDisableLoopback(getMibSpiBase());
    mibspiGetData(getMibSpiBase(), group, data);
}

void LibWrapMibSpi::transfer(uint32 group)
{
    if (group == LibWrapMibSpi::LOOP_BACK_TEST) {
        mibspiEnableLoopback(getMibSpiBase(), Digital_Lbk);
    }
    mibspiEnableGroupNotification(getMibSpiBase(), group, 0);
    mibspiTransfer(getMibSpiBase(), group);
}

bool LibWrapMibSpi::waitForTransferComplete(uint32 group, int msTimeout)
{
    if (xSemaphoreTake(getSem(), pdMS_TO_TICKS(msTimeout)) == pdFALSE) {
        return false;
    }
    return true;
}

void LibWrapMibSpi::lock(int msTimeout)
{
    xSemaphoreTake(getMibSpiMutex(), pdMS_TO_TICKS(msTimeout));
}

void LibWrapMibSpi::unlock()
{
    xSemaphoreGive(getMibSpiMutex());
}

void LibWrapMibSpi::notification(uint32 group)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(getSem(), &xHigherPriorityTaskWoken);
}

extern "C" void mibspiGroupNotification(mibspiBASE_t* mibspiReg, uint32 group)
{
    if (LibWrapMibSpi::s_notificationMap->find(mibspiReg)
                                      != LibWrapMibSpi::s_notificationMap->end()
   && (*LibWrapMibSpi::s_notificationMap)[mibspiReg]) {
        mibspiDisableGroupNotification(mibspiReg, group);
        (*LibWrapMibSpi::s_notificationMap)[mibspiReg]->notification(group);
    }
}
