#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

#include <map>
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "libAdc.h"

class LibThermistor
{
public:
    enum Type {
        USP12837,
        SC30F103AN,
        NTCG163JF103FT1,
    };
    enum Units {
        CELSIUS,
        FAHRENHEIT,
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
    struct Conversion {
        float rt;
        float temp[2];
    };
public:
    LibThermistor();
    LibThermistor(int type, int units);
    virtual ~LibThermistor();
    float getTemperature(float rt);
    int setType(int type);
    int getType();
    int setUnits(int units);
    int getUnits();
private:
    SemaphoreHandle_t m_mutex;
    static struct Conversion s_convTableUSP12837[];
    static struct Conversion s_convTableSC30F103AN[];
    static struct Conversion s_convTableNTCG163JF103FT1[];
    int m_units;
    int m_type;
    static struct Conversion* s_types[];
// TODO: Put into a separate class
public:
    enum Channel {
        AIN_A,
        AIN_B,
        AIN_C,
        AIN_D,
    };
public:
    int readTemp(int channel, float& value);  // degC/degF
    int readTemp(int channel, uint32& value); // 0-65535
private:
    enum adcChannels {
        TEMP_AIN_A = LibAdc::CHANNEL_2,
        TEMP_AIN_B = LibAdc::CHANNEL_3,
        TEMP_AIN_C = LibAdc::CHANNEL_4,
        TEMP_AIN_D = LibAdc::CHANNEL_5,
    };
private:
    float convertVoltageToResistance(float ain);
private:
    LibAdc m_libAdc;
};

#endif // _LIB_THERMISTOR_H_
