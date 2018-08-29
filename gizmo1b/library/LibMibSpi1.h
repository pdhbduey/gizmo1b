#pragma once

#include <FreeRTOS.h>
#include <os_semphr.h>
#include <reg_mibspi.h>
#include <LibMibSpi.h>

class LibMibSpi1 : public LibMibSpi
{
public:
    enum SomiSelect {
        SPI_A,
        SPI_B,
    };
    enum Group {
         // BB
        DAC8563_16_BIT_DAC         = GROUP_1,
        AD7699_16_BIT_ADC          = GROUP_2,
        L6470_STEPPER_MOTOR_DRIVER = GROUP_3,
        // Thermal Board
        AD5683_16_BIT_DAC          = GROUP_1,
        ADS8330_16_BIT_ADC         = GROUP_4,
    };
    LibMibSpi1();
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
