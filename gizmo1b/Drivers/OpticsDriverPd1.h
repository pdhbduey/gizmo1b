#pragma once

#include <OpticsDriver.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class OpticsDriverPd1 : public OpticsDriver
{
public:
    OpticsDriverPd1(uint32_t nSiteIdx = 0);
    virtual void SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity);
    void OpticsDriverInit();
};

