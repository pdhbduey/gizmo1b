#pragma once

#include <map>
#include <FreeRTOS.h>
#include <os_semphr.h>
#include <LibAdc.h>
#include <LibThermistorCurves.h>

class LibThermistor
{
public:
    enum Status {
        OKAY,
        ERROR_INVALID_TEMP_CHANNEL,
        ERROR_READ_TEMP_AIN_A,
        ERROR_READ_TEMP_AIN_B,
        ERROR_READ_TEMP_AIN_C,
        ERROR_READ_TEMP_AIN_D,
        ERROR_READ_TEMP_INA1,
        ERROR_READ_TEMP_INA2,
        ERROR_READ_TEMP_INB1,
        ERROR_READ_TEMP_INB2,
    };
    enum Channel {
        AIN_A,
        AIN_B,
        AIN_C,
        AIN_D,
        INA1,
        INA2,
        INB1,
        INB2,
    };
public:
    LibThermistor(LibAdc* libAdc, int type, int units);
    int readTemp(int channel, float& value);  // degC/degF
    int readTemp(int channel, uint32& value); // 0-65535
    int setType(int type);
    int getType();
    int setUnits(int units);
    int getUnits();
protected:
    virtual float convertVoltageToResistance(float ain) = 0;
protected:
    std::map<int, int> m_channelToAdcInMap;
private:
    LibAdc& m_libAdc;
    LibThermistorCurves m_libThermistorCurves;
    SemaphoreHandle_t m_mutex;
};
