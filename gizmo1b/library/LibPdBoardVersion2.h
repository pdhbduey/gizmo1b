#pragma once

#include <map>
#include <LibPdBoard.h>
#include <OpticsDriverPd2.h>

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
private:
    float m_refV;
    OpticsDriverPd2 m_opticsDriverPd2;
    uint32 m_photodiode;
    std::map<int, int> m_pdToAdcMap;
    uint32 m_photodiodeResultRaw;
    uint32 m_integrationTimeInUs;
};

