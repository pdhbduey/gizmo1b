#pragma once

#include <FreeRTOS.h>
#include <os_semphr.h>
#include <reg_mibspi.h>
#include <libWrapMibSpi.h>

class LibMibSpi3 : public LibWrapMibSpi
{
public:
    enum SomiSelect {
        SPI_A,
        SPI_B,
    };
    enum Group {
        AD7699BCPZ_8CH_16BIT_ADC = GROUP_1,
    };
    LibMibSpi3();
    virtual ~LibMibSpi3();
    void somiSelect(int somi);
private:
    virtual mibspiBASE_t* getMibSpiBase();
    virtual SemaphoreHandle_t& getMibSpiMutex();
    virtual SemaphoreHandle_t& getSem();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mibSpiMutex;
    mibspiBASE_t* m_mibSpiBase;
    static SemaphoreHandle_t s_sem;
    LibWrapGioPort::Port m_somiSw;
};