#pragma once

#include <FreeRTOS.h>
#include <os_semphr.h>
#include <reg_mibspi.h>
#include <LibMibSpi.h>

class LibMibSpi5 : public LibMibSpi
{
public:
    enum Group {
        AD7699_16BIT_ADC = GROUP_1,
    };
public:
    LibMibSpi5();
private:
    virtual mibspiBASE_t* getMibSpiBase();
    virtual SemaphoreHandle_t& getMibSpiMutex();
    virtual SemaphoreHandle_t& getSem();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mibSpiMutex;
    mibspiBASE_t* m_mibSpiBase;
    static SemaphoreHandle_t s_sem;
};
