#pragma once

#include "OpticsDriver.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class OpticsDriverPd2 : public OpticsDriver
{
public:
    OpticsDriverPd2(uint32_t nSiteIdx = 0);
    virtual void SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity);
    void OpticsDriverInit();
};
