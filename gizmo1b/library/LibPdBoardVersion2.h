#pragma once

#include <map>
#include <LibPdBoard.h>
#include <OpticsDriverPd2.h>
#include <LibThermistorCurves.h>
#include <libWrapGioPort.h>

class LibPdBoardVersion2 : public LibPdBoard
{
public:
    LibPdBoardVersion2();
    virtual ~LibPdBoardVersion2();
    virtual void setPhotodiode(uint32 photodiode);
    virtual uint32 getPhotodiode();
    virtual float readPhotodiodeResult();     // 0-Vref
    virtual uint32 readPhotodiodeResultRaw(); // 0-65535
    virtual uint32 getVersion();
    virtual uint32 getIntegrationTimeInUs(); // 1,000us-1,000,000us
    virtual void setIntegrationTimeInUs(uint32 integrationTimeInUs);
    virtual float readPhotodiodeTemperature(); // degC
    virtual float readPhotodiodeTemperatureDuringIntegration(); // degC
private:
    enum TempSwCCtrlPins {
        TEMP_SW_CTRL_A,
        TEMP_SW_CTRL_B,
    };
private:
    float convertRawToDeg(uint16_t data);
private:
    float m_refV;
    OpticsDriverPd2 m_opticsDriverPd2;
    uint32 m_photodiode;
    std::map<int, int> m_pdToAdcMap;
    uint32 m_photodiodeResultRaw;
    uint32 m_integrationTimeInUs;
    float m_photodiodeTemperatureDuringIntegration;
    LibThermistorCurves m_photodiodeThermistorCurve;
    std::map<int, int> m_pdTempToAdcMap;
    std::map<int, LibWrapGioPort::Port*> m_tempSwCCtrlPinsMap;
};

