#include <LibMibSpi.h>
#include <mibspi.h>

bool LibMibSpi::s_isInitialized;
std::map<mibspiBASE_t*, LibMibSpi*>* LibMibSpi::s_notificationMap;
SemaphoreHandle_t LibMibSpi::s_mibSpiMutex;

LibMibSpi::LibMibSpi()
{
    if (!s_isInitialized) {
        //mibspiInit();
        s_notificationMap = new std::map<mibspiBASE_t*, LibMibSpi*>;
        s_mibSpiMutex =  xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibMibSpi::~LibMibSpi()
{
}

void LibMibSpi::addNotification(LibMibSpi* libWrapMibSpi)
{
    xSemaphoreTake(s_mibSpiMutex, portMAX_DELAY);
    (*s_notificationMap)[libWrapMibSpi->getMibSpiBase()] = libWrapMibSpi;
    xSemaphoreGive(s_mibSpiMutex);
}

void LibMibSpi::setData(uint32 group, uint16* data)
{
    mibspiSetData(getMibSpiBase(), group, data);
}

void LibMibSpi::getData(uint32 group, uint16* data)
{
    mibspiDisableLoopback(getMibSpiBase());
    mibspiGetData(getMibSpiBase(), group, data);
}

void LibMibSpi::transfer(uint32 group)
{
    if (group == LibMibSpi::LOOP_BACK_TEST) {
        mibspiEnableLoopback(getMibSpiBase(), Digital_Lbk);
    }
    mibspiEnableGroupNotification(getMibSpiBase(), group, 0);
    mibspiTransfer(getMibSpiBase(), group);
}

bool LibMibSpi::waitForTransferComplete(uint32 group, int msTimeout)
{
    if (xSemaphoreTake(getSem(), pdMS_TO_TICKS(msTimeout)) == pdFALSE) {
        return false;
    }
    return true;
}

void LibMibSpi::lock(int msTimeout)
{
    xSemaphoreTake(getMibSpiMutex(), pdMS_TO_TICKS(msTimeout));
}

void LibMibSpi::unlock()
{
    xSemaphoreGive(getMibSpiMutex());
}

LibMibSpi::Lock::Lock(LibMibSpi& libWrapMibSpi) :
    m_libWrapMibSpi(libWrapMibSpi)
{
    m_libWrapMibSpi.lock();
}

LibMibSpi::Lock::~Lock()
{
    m_libWrapMibSpi.unlock();
}

void LibMibSpi::notification(uint32 group)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(getSem(), &xHigherPriorityTaskWoken);
}

extern "C" void mibspiGroupNotification(mibspiBASE_t* mibspiReg, uint32 group)
{
    if (*LibMibSpi::s_notificationMap->find(mibspiReg)
                                     != *LibMibSpi::s_notificationMap->end()
     && (*LibMibSpi::s_notificationMap)[mibspiReg]) {
        mibspiDisableGroupNotification(mibspiReg, group);
        (*LibMibSpi::s_notificationMap)[mibspiReg]->notification(group);
    }
}
