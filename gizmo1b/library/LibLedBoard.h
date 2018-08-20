#pragma once

#include <hal_stdtypes.h>

class LibLedBoard
{
public:
    enum Control {
        SELECT_LED_1    = 1,
        SELECT_LED_2    = 2,
        SELECT_LED_3    = 3,
        SELECT_LED_4    = 4,
        SELECT_LED_5    = 5,
        SELECT_LED_6    = 6,

        SELECT_LED_MASK = 7,
    };
    enum BoardVersion {
        LED_BOARD_V1   = 1 << 0,
        LED_BOARD_V2   = 1 << 1,
    };
public:
    LibLedBoard();
    virtual ~LibLedBoard();
    virtual void setLed(uint32 led) = 0;
    virtual uint32 getLed() = 0;
    virtual float readLedTemperature() = 0;                  // degC
    virtual float readLedTemperatureDuringIntegration() = 0; // degC
    virtual uint32 getVersion() = 0;
    virtual uint32 getLedIntensity() = 0; // 0-40,000
    virtual void setLedIntensity(uint32 ledIntensity) = 0;
    virtual float readLedMonitorPhotodiode() = 0;                         // 0V-Vref
    virtual float readLedMonitorPhotodiodeDuringIntegration() = 0;        // 0V-Vref
    virtual float readPhotodiodeResult(uint32_t integrationTimeInUs) = 0; // 0V-Vref
    virtual void turnLedOn() = 0;
    virtual void turnLedOff() = 0;
};
