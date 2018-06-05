#ifndef _LIB_PHOTODIODE_H_
#define _LIB_PHOTODIODE_H_

#include <map>
#include "FreeRTOS.h"
#include "os_semphr.h"
#include "OpticsDriver.h"

class LibPhotodiode
{
public:
    enum Control {
        SELECT_LED_BLUE1 = 1,
        SELECT_LED_GREEN,
        SELECT_LED_RED1,
        SELECT_LED_BROWN,
        SELECT_LED_RED2,
        SELECT_LED_BLUE2,
        SELECT_LED_MASK = 7,
        SELECT_PHOTODIODE_D11_T1 = 1 << 3,
        SELECT_PHOTODIODE_D10_T1 = 2 << 3,
        SELECT_PHOTODIODE_D11_T2 = 3 << 3,
        SELECT_PHOTODIODE_D10_T2 = 4 << 3,
        SELECT_PHOTODIODE_D11_T3 = 5 << 3,
        SELECT_PHOTODIODE_D10_T3 = 6 << 3,
        SELECT_PHOTODIODE_MASK   = 7 << 3
    };
    enum Status {
        OKAY,
        ERROR_INTEGRATION_TIME_OUT_OF_RANGE,
        ERROR_SELECT_LED_OUT_OF_RANGE,
        ERROR_SELECT_PHOTODIODE_OUT_OF_RANGE,
        ERROR_LED_INTENSITY_OUT_OF_RANGE,
    };
public:
    LibPhotodiode();
    virtual ~LibPhotodiode();
    int setLed(int led);
    int setPhotodiode(int photodiode);
    int getLed();
    int getPhotodiode();
    int getIntegrationTimeInUs(); // 10,000us-1,000,000us
    int setIntegrationTimeInUs(int integrationTimeInUs);
    float getLedIntensity(); // 0A-1A
    int setLedIntensity(float ledIntensity);
    float readPhotodiode();
private:
    OpticsDriver m_opticsDriver;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    int m_led;
    int m_photodiode;
    int m_integrationTimeInUs;
    float m_ledIntensity;
    std::map<int, int> m_ledMap;
    std::map<int, int> m_pdMap;
};

#endif // _LIB_PHOTODIODE_H_
