#pragma once

#include <map>
#include <FreeRTOS.h>
#include <os_semphr.h>
#include <LibPdBoardVersion2.h>
#include <LibLedBoardVersion2.h>

class LibPhotodiode
{
public:
    enum Control {
        LED_BOARD_ENABLED  = 1 << 6,
        LED_BOARD_DISABLED = 1 << 7,
        LED_BOARD_MASK     = 3 << 6,
        PD_BOARD_ENABLED   = 1 << 8,
        PD_BOARD_DISABLED  = 1 << 9,
        PD_BOARD_MASK      = 3 << 8,
        LED_TURN_ON        = 1 << 10,
        LED_TURN_OFF       = 1 << 11,
        LED_STATE_MASK     = 3 << 10,
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
public:
    LibPhotodiode();
    virtual ~LibPhotodiode();
    int setLed(int led);
    int setPhotodiode(uint32 photodiode);
    int getLed();
    uint32 getPhotodiode();
    uint32 getIntegrationTimeInUs(); // 1,000us-1,000,000us
    int setIntegrationTimeInUs(uint32 integrationTimeInUs);
    uint32 getLedIntensity(); // 0-40,000
    int setLedIntensity(uint32 ledIntensity);
    float readPhotodiode();     // 0V-Vref
    uint32 readPhotodiodeRaw(); // 0-65,535
    uint32 readLedBoardVersion();
    uint32 readPhotodiodeBoardVersion();
    int setLedBoardVersion(uint32 version);
    int setPhotodiodeBoardVersion(uint32 version);
    float readLedTemperature();        // degC
    float readPhotodiodeTemperature(); // degC
    float readPhotodiodeTemperatureDuringIntegration(); // degC
    void ledBoardEnable();
    void ledBoardDisable();
    void pdBoardEnable();
    void pdBoardDisable();
    uint32 getLedBoardEnabledStatus();
    uint32 getPhotodiodeBoardEnabledStatus();
    float readLedMonitorPhotodiode();  				   // 0V-Vref
    float readLedMonitorPhotodiodeDuringIntegration(); // 0V-Vref
    float readLedTemperatureDuringIntegration();       // degC
    uint32 getLedState();
    void ledTurnOn();
    void ledTurnOff();
private:
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    uint32 m_ledState;
    LibPdBoard* m_libPdBoard;
    LibLedBoard* m_libLedBoard;
    uint32 m_led;
    uint32 m_photodiode;
    uint32 m_ledBoardVersion;
    uint32 m_pdBoardVersion;
    bool m_isLedBoardEnabled;
    bool m_isPdBoardEnabled;
    uint32 m_integrationTimeInUs;
    uint32 m_ledIntensity;
};
