#pragma once

#include <map>
#include <LibLedBoard.h>
#include <OpticsDriverLed2.h>
#include <LibThermistorCurves.h>

class LibLedBoardVersion2 : public LibLedBoard
{
public:
    LibLedBoardVersion2();
    virtual ~LibLedBoardVersion2();
    virtual void setLed(uint32 led);
    virtual uint32 getLed();
    virtual float readLedTemperature();                  // degC
    virtual float readLedTemperatureDuringIntegration(); // degC
    virtual uint32 getVersion();
    virtual uint32 getLedIntensity(); // 0-40,000
    virtual void setLedIntensity(uint32 ledIntensity);
    virtual float readLedMonitorPhotodiode();                         // 0V-Vref
    virtual float readLedMonitorPhotodiodeDuringIntegration();        // 0V-Vref
    virtual float readPhotodiodeResult(uint32_t integrationTimeInUs); // 0V-Vref
    virtual void turnLedOn();
    virtual void turnLedOff();
private:
    enum LedCtrlSwPins {
        LED_CTRL_S0,
        LED_CTRL_S1,
        LED_CTRL_S2,
    };
private:
    float convertRawToDeg(uint32_t data);
private:
    float m_refV;
    OpticsDriverLed2 m_opticsDriverLed2;
    uint32 m_led;
    std::map<int, int> m_ledTempToAdcMap;
    float m_ledTemperatureDuringIntegration;
    LibThermistorCurves m_ledThermistorCurve;
    uint32 m_ledMonPdAdcChannel;
    uint32 m_ledVisenseAdcChannel;
    uint32 m_ledIntensity;
    std::map<int, int> m_ledToDacMap;
    float m_ledMonPdDuringIntegration;
    std::map<int, LibWrapGioPort::Port*> m_ledCtrlSwPinsMap;
};

