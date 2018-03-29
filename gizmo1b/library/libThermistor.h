#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "libAdc.h"

class LibThermistor
{
public:
    enum Channel {
        AIN_A,
        AIN_B,
        AIN_C,
        AIN_D,
    };
    enum Status {
        OKAY,
        ERROR_INVALID_TEMP_CHANNEL,
        ERROR_READ_TEMP_AIN_A,
        ERROR_READ_TEMP_AIN_B,
        ERROR_READ_TEMP_AIN_C,
        ERROR_READ_TEMP_AIN_D,
    };
public:
    LibThermistor();
    virtual ~LibThermistor();
    // Calculated Temp Based on Thermistor resistance table
    int readTemp(int channel, float& value);
    static void test();
private:
    enum adcChannels {
        TEMP_AIN_A = LibAdc::CHANNEL_2,
        TEMP_AIN_B = LibAdc::CHANNEL_3,
        TEMP_AIN_C = LibAdc::CHANNEL_4,
        TEMP_AIN_D = LibAdc::CHANNEL_5,
    };
    enum Temp {
        CELSIUS,
        FAHRENHEIT,
    };
    struct conversion {
        float rt;
        float temp[2];
    };
private:
    float convertVoltageToTemp(float ain, int standard);
private:
    LibAdc m_libAdc;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    static struct conversion s_convTable[];
};

#endif // _LIB_THERMISTOR_H_
