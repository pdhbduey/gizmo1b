#pragma once

#include <map>
#include <libWrapGioPort.h>
#include <LibDac.h>
#include "LibMibSpi.h"

class LibAD5683 : public LibDac
{
public:
    enum DacCtrl {
        SYNC,
    };
    enum DacCommands {
        DAC_AD5683R_DO_NOTHING,
        DAC_AD5683R_WRITE_INPUT_REGISTER,
        DAC_AD5683R_UPDATE_DAC_REGISTER,
        DAC_AD5683R_WRITE_DAC_AND_INPUT_REGISTER,
        DAC_AD5683R_WRITE_CTRL_REGISTER,
        DAC_AD5683R_READ_INPUT_REGISTER,
    };
    enum DacCtrlReg {
        DAC_AD5683R_RESET       = 1 << 19,
        DAC_AD5683R_NORMAL_MODE = 0 << 17,
        DAC_AD5683R_ENABLE_REF  = 0 << 16,
        DAC_AD5683R_DISABLE_REF = 1 << 16,
        DAC_AD5683R_OUT_1xVREF  = 0 << 15,
        DAC_AD5683R_OUT_2xVREF  = 1 << 15,
        DAC_AD5683R_STANDALONE  = 0 << 14,
        DAC_AD5683R_CHAINED     = 1 << 14,
    };
public:
    LibAD5683(LibMibSpi* libWrapMibSpi, uint32 mibSpiGroup, float refV = 5.0);
    // LibDac
    virtual int set(float value);
    virtual int set(uint32 value);
    void initialize();
private:
    int writeDac(uint16* txBuffer);
protected:
    LibWrapGioPort::Port* m_dacSync;
private:
    LibMibSpi& m_libWrapMibSpi;
    uint32 m_mibSpiGroup;
    float m_refV;
};
