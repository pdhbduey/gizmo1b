#pragma once

#include <FreeRTOS.h>
#include <os_semphr.h>

class LibThermistorCurves
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
        ERROR_INVALID_TYPE = 6,
        ERROR_INVALID_UNITS,
    };
    struct Conversion {
        float rt;
        float temp[2];
    };
public:
    LibThermistorCurves(int type, int units);
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
};
