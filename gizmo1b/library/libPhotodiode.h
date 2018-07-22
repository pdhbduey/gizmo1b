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
        ERROR_LED_BOARD_VERSION_INVALID,
        ERROR_PHOTODIODE_BOARD_VERSION_INVALID,
    };
    enum LedBoardVersion {
        LED_BOARD_V1 = 1 << 0,
        LED_BOARD_V2 = 1 << 1,
    };
    enum PhotodiodeBoardVersion {
        PHOTODIODE_BOARD_V1 = 1 << 0,
        PHOTODIODE_BOARD_V2 = 1 << 1,
    };
    enum TemperatureUnits {
        CELSIUS,
        FAHRENHEIT,
    };
public:
    LibPhotodiode();
    virtual ~LibPhotodiode();
    int setLed(int led);
    int setPhotodiode(int photodiode);
    int getLed();
    int getPhotodiode();
    uint32 getIntegrationTimeInUs(); // 1,000us-1,000,000us
    int setIntegrationTimeInUs(uint32 integrationTimeInUs);
    uint32 getLedIntensity(); // 0-40,000
    int setLedIntensity(uint32 ledIntensity);
    float readPhotodiode(); // 0V-5V
    uint32 readPhotodiodeRaw(); // 0-65,535
    uint32 readLedBoardVersion();
    uint32 readPhotodiodeBoardVersion();
    int setLedBoardVersion(uint32 version);
    int setPhotodiodeBoardVersion(uint32 version);
    float readLedTemperature();        // degC
    float readPhotodiodeTemperature(); // degC
    float readLedMonitorPhotodiode();  // 0V-5V
    float readLedTemperatureDuringIntegration();        // degC
    float readPhotodiodeTemperatureDuringIntegration(); // degC
private:
    OpticsDriver m_opticsDriver;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    int m_led;
    int m_photodiode;
    uint32 m_integrationTimeInUs;
    uint32 m_ledIntensity;
    std::map<int, int> m_ledMap;
    std::map<int, int> m_pdMap;
    uint32 m_photodiodeResultRaw;
    int m_temperatureUnits;
    int m_ledBoardVersion;
    int m_photodiodeBoardVersion;
};

#endif // _LIB_PHOTODIODE_H_
