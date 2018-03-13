#ifndef _LIB_ADC_H_
#define _LIB_ADC_H_

class LibAdc
{
public:
    enum Channel {
        CHANNEL_0,
        CHANNEL_1,
        CHANNEL_2,
        CHANNEL_3,
        CHANNEL_4,
        CHANNEL_5,
    };
    enum Status {
        OKAY,
        ERROR_INVALID_CHANNEL,
    };
    enum ConversionStatus {
        IDLE,
        BUSY,
        DONE,
        ERROR_TIMEOUT,
    };
    LibAdc();
    virtual ~LibAdc();
    int read(int channel, float& value); // 0-5V
    int startConversion(int channel);
    int getStatus();
    float getResult();
private:
    static bool s_isInitialized;
    float m_result;
    int m_status;
};

#endif // _LIB_ADC_H_
