#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

#include <map>
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
        ERROR_INVALID_TYPE,
        ERROR_INVALID_UNITS,
    };
    enum Type {
        USP12837,
        SC30F103AN,
    };
    enum Units {
        CELSIUS,
        FAHRENHEIT,
    };
public:
    LibThermistor();
    virtual ~LibThermistor();
    // Calculated Temp Based on Thermistor resistance table
    int readTemp(int channel, float& value);  // degC/degF
    int readTemp(int channel, uint32& value); // 0-65535
    int setType(int type);
    int getType();
    int setUnits(int units);
    int getUnits();
private:
    enum adcChannels {
        TEMP_AIN_A = LibAdc::CHANNEL_2,
        TEMP_AIN_B = LibAdc::CHANNEL_3,
        TEMP_AIN_C = LibAdc::CHANNEL_4,
        TEMP_AIN_D = LibAdc::CHANNEL_5,
    };
    struct Conversion {
        float rt;
        float temp[2];
    };
private:
    float convertVoltageToTemp(float ain, int standard);
private:
    LibAdc m_libAdc;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    static struct Conversion s_convTableUSP12837[];
    static struct Conversion s_convTableSC30F103AN[];
    int m_units;
    int m_type;
    static struct Conversion* s_types[];
};

#endif // _LIB_THERMISTOR_H_
