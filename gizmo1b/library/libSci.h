#ifndef _LIB_COMM_H_
#define _LIB_COMM_H_

#include <vector>
#include <map>
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "os_queue.h"
#include "sys_common.h"
#include "sci.h"

class LibSci {
public:
    enum BaudRate {
        BAUD_9600,
        BAUD_19200,
        BAUD_38400,
        BAUD_57600,
        BAUD_115200,
    };
    enum Parity {
        NONE,
        EVEN,
        ODD,
    };
    enum StopBits {
        ONE,
        TWO,
    };
    enum Status {
        OKAY,
        ERROR_BAUD_RATE,
        ERROR_PARITY,
        ERROR_STOP_BITS,
        ERROR_WRITE,
        ERROR_READ,
    };
    LibSci();
    virtual ~LibSci();
    void open();
    void close();
    int setBaudRate(int baudRate);
    int setParity(int parity);
    int setStopBits(int stopBits);
    int read(std::vector<uint8>& data);
    int write(const std::vector<uint8>& data);
    bool isDataWritten(int msTimeout = 1000);
    bool isDataAvailable(int msTimeout = 1000);
    friend void sciNotification(sciBASE_t* sci, uint32 flags);
protected:
    virtual void openLowLevel() = 0;
    virtual void closeLowLevel() = 0;
    virtual void clearErrorsLowLevel() = 0;
    virtual void writeDataLowLevel(uint8 data) = 0;
    virtual uint8 readDataLowLevel() = 0;
    virtual void enableTxIntLowLevel() = 0;
    virtual QueueHandle_t getRxQueue() = 0;
    virtual QueueHandle_t getTxQueue() = 0;
    virtual SemaphoreHandle_t getSem() = 0;
    virtual sciBASE_t* getSciBaseReg() = 0;
    void addNotification(LibSci* libSci);
private:
    void notification(uint32 flags);
protected:
    int m_baudRate;
    int m_parity;
    int m_stopBits;
private:
    static bool s_isInitialized;
    std::map<sciBASE_t*, LibSci*> m_notificationMap;
    static std::map<sciBASE_t*, LibSci*>* s_notificationMap;
};

#endif // _LIB_COMM_H_
