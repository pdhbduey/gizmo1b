#pragma once

#include <map>
#include <LibLedBoard.h>
#include <LibThermistor.h>
#include <LibAdcLedBoardVersion2.h>

class LibLedBoardVersion2 : public LibLedBoard
{
public:
    LibLedBoardVersion2();
    virtual ~LibLedBoardVersion2();
    virtual int setLed(uint32 led);
    virtual uint32 getLed();
    virtual float readLedTemperature();
    virtual uint32 getVersion();
private:
    float convertLedTemperatureRawDataToResistance(uint32_t data);
private:
    LibAdcLedBoardVersion2 m_libAdcLedBoardVersion2;
    uint32 m_led;
    std::map<int, int> m_ledToAdcMap;
    uint32 m_ledTemperature;
    LibThermistor m_ledThermistor;
};

