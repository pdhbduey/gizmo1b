#pragma once

#include <hal_stdtypes.h>

class LibPdBoard
{
public:
    enum Control {
        SELECT_PHOTODIODE_1    = 1 << 3,
        SELECT_PHOTODIODE_2    = 2 << 3,
        SELECT_PHOTODIODE_3    = 3 << 3,
        SELECT_PHOTODIODE_4    = 4 << 3,
        SELECT_PHOTODIODE_5    = 5 << 3,
        SELECT_PHOTODIODE_6    = 6 << 3,

        SELECT_PHOTODIODE_MASK = 7 << 3,
    };
    enum BoardVersion {
        PHOTODIODE_BOARD_V1   = 1 << 0,
        PHOTODIODE_BOARD_V2   = 1 << 1,
    };
public:
    LibPdBoard();
    virtual ~LibPdBoard();
    virtual void setPhotodiode(uint32 photodiode) = 0;
    virtual uint32 getPhotodiode() = 0;
    virtual float readPhotodiodeResult() = 0; // 0V-Vref
    virtual uint32 readPhotodiodeResultRaw() = 0; // 0-65,535
    virtual uint32 getVersion() = 0;
    virtual uint32 getIntegrationTimeInUs() = 0; // 1,000us-1,000,000us
    virtual void setIntegrationTimeInUs(uint32 integrationTimeInUs) = 0;
    virtual float readPhotodiodeTemperature() = 0; // degC
    virtual float readPhotodiodeTemperatureDuringIntegration() = 0; // degC
};
