#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

class LibThermistor
{
public:
    enum LibThermistorTempChannel {
        AIN_A,
        AIN_B,
        AIN_C,
        AIN_D,
    };
    enum LibThermistorStatus {
        OKAY,
        ERROR_INVALID_TEMP_CHANNEL,
    };
    enum ConversionStatus {
        IDLE,
        BUSY,
        DONE,
        ERROR_TIMEOUT,
    };
    LibThermistor();
    virtual ~LibThermistor();
    // Calculated Temp Based on Thermistor resistance table
    int readTemp(int channel, float& value);
    int startReading(int channel);
    int getStatus();
    float getResult();
private:
    static bool s_isInitialized;
    float m_result;
    int m_status;
};

#endif // _LIB_THERMISTOR_H_
