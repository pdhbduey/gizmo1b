#pragma once

#include <hal_stdtypes.h>

class LibLedBoard
{
public:
    enum Control {
        SELECT_LED_BLUE1 = 1,
        SELECT_LED_GREEN,
        SELECT_LED_RED1,
        SELECT_LED_BROWN,
        SELECT_LED_RED2,
        SELECT_LED_BLUE2,
        SELECT_LED_MASK,
    };
    enum Status {
        OKAY,
        ERROR_SELECT_LED_OUT_OF_RANGE = 2,
    };
    enum BoardVersion {
        LED_BOARD_V1   = 1 << 0,
        LED_BOARD_V2   = 1 << 1,
    };
public:
    LibLedBoard();
    virtual ~LibLedBoard();
    virtual int setLed(uint32 led) = 0;
    virtual uint32 getLed() = 0;
    virtual float readLedTemperature() = 0; // degC
    virtual uint32 getVersion() = 0;
};
