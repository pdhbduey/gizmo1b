#include "sci.h"
#include "boardTestFastPidApp.h"
#include "libSci.h"

bool LibSci::s_isInitialized;
std::map<sciBASE_t*, LibSci*>* LibSci::s_notificationMap;

LibSci::LibSci()
{
    if (!s_isInitialized) {
        sciInit();
        s_notificationMap = &m_notificationMap;
        s_isInitialized = true;
    }
}

LibSci::~LibSci()
{
}

void LibSci::open()
{
    openLowLevel();
    xSemaphoreGive(getSem());
}

void LibSci::close()
{
    closeLowLevel();
    xQueueReset(getRxQueue());
    xQueueReset(getTxQueue());
}

int LibSci::setBaudRate(int baudRate)
{
    switch (baudRate) {
    case BAUD_9600:
    case BAUD_19200:
    case BAUD_38400:
    case BAUD_57600:
    case BAUD_115200:
        m_baudRate = baudRate;
        return OKAY;
    default:
        return ERROR_BAUD_RATE;
    }
}

int LibSci::setParity(int parity)
{
    switch (parity) {
    case BAUD_9600:
    case BAUD_19200:
    case BAUD_38400:
    case BAUD_57600:
    case BAUD_115200:
        m_parity = parity;
        return OKAY;
    default:
        return ERROR_PARITY;
    }
}

int LibSci::setStopBits(int stopBits)
{
    switch (stopBits) {
    case BAUD_9600:
    case BAUD_19200:
    case BAUD_38400:
    case BAUD_57600:
    case BAUD_115200:
        m_stopBits = stopBits;
        return OKAY;
    default:
        return ERROR_STOP_BITS;
    }
}

int LibSci::read(std::vector<uint8>& data)
{
    uint8 dat;
    if (xQueuePeek(getRxQueue(), &dat, 0) == errQUEUE_EMPTY) {
        return ERROR_READ;
    }
    while (xQueueReceive(getRxQueue(), &dat, 0) == pdPASS) {
        data.push_back(dat);
    }
    return OKAY;
}

int LibSci::write(const std::vector<uint8>& data)
{
    xQueueReset(getTxQueue());
    if (uxQueueSpacesAvailable(getTxQueue()) < data.size()) {
        return ERROR_WRITE;
    }
    for (int i = 1; i < data.size(); i++) {
        uint8 dat = data[i];
        xQueueSend(getTxQueue(), &dat, 0);
    }
    writeDataLowLevel(data[0]);
    enableTxIntLowLevel();
    return OKAY;
}

bool LibSci::isDataWritten(int msTimeout)
{
    if (xSemaphoreTake(getSem(), pdMS_TO_TICKS(msTimeout)) == pdFALSE) {
        return false;
    }
    clearErrorsLowLevel();
    return true;
}

bool LibSci::isDataAvailable(int msTimeout)
{
    uint8 dat;
    return xQueuePeek(getRxQueue(), &dat, pdMS_TO_TICKS(msTimeout))
                                                              != errQUEUE_EMPTY;
}

void LibSci::addNotification(LibSci* libSci)
{
    m_notificationMap[getSciBaseReg()] = libSci;
}

void LibSci::notification(uint32 flags)
{
    uint8 byte;
    if (flags & SCI_RX_INT) {
        byte = (uint8)(getSciBaseReg()->RD & 0x000000FFU);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(getRxQueue(), &byte, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    if  (flags & SCI_TX_INT) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (xQueueReceiveFromISR(getTxQueue(), &byte,
                                 &xHigherPriorityTaskWoken) != errQUEUE_EMPTY) {
            getSciBaseReg()->TD = (uint32)byte;
        }
        else {
            getSciBaseReg()->CLEARINT = (uint32)SCI_TX_INT;
            xHigherPriorityTaskWoken = pdFALSE;
            xSemaphoreGiveFromISR(getSem(), &xHigherPriorityTaskWoken);
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

extern "C" void sciNotification(sciBASE_t* sci, uint32 flags)
{
    if (BoardTestFastPidApp::s_isBoardTestFastPidApp) {
        BoardTestFastPidApp::sciNotification(sci, flags);
        return;
    }
    if (LibSci::s_notificationMap->find(sci) != LibSci::s_notificationMap->end()
   && (*LibSci::s_notificationMap)[sci]) {
        (*LibSci::s_notificationMap)[sci]->notification(flags);
    }
}
