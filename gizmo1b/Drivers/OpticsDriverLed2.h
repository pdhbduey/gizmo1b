#pragma once

#include "OpticsDriver.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class OpticsDriverLed2 : public OpticsDriver
{
public:
    OpticsDriverLed2(uint32_t nSiteIdx = 0);
    virtual void SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity);
    void OpticsDriverInit();
private:
    enum DacAD568rCommands {
        DAC_AD5683R_DO_NOTHING,
        DAC_AD5683R_WRITE_INPUT_REGISTER,
        DAC_AD5683R_UPDATE_DAC_REGISTER,
        DAC_AD5683R_WRITE_DAC_AND_INPUT_REGISTER,
        DAC_AD5683R_WRITE_CTRL_REGISTER,
        DAC_AD5683R_READ_INPUT_REGISTER,
    };
    enum DacAD568rCtrlReg {
        DAC_AD5683R_RESET       = 1 << 19,
        DAC_AD5683R_NORMAL_MODE = 0 << 17,
        DAC_AD5683R_ENABLE_REF  = 0 << 16,
        DAC_AD5683R_DISABLE_REF = 1 << 16,
        DAC_AD5683R_OUT_1xVREF  = 0 << 15,
        DAC_AD5683R_OUT_2xVREF  = 1 << 15,
        DAC_AD5683R_STANDALONE  = 0 << 14,
        DAC_AD5683R_CHAINED     = 1 << 14,
    };
};
