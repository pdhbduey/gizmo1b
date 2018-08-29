#pragma once

#include <libWrapGioPort.h>
#include <LibAdc.h>
#include <LibMibSpi.h>

class LibAD7699 : public LibAdc
{
public:
    enum CtrlRegisterShifts {
        CFG_SHIFT         = 13,
        IN_CH_CFG_SHIFT   = 10,
        IN_CH_SEL_SHIFT   =  7,
        FULL_BW_SEL_SHIFT =  6,
        REF_SEL_SHIFT     =  3,
        SEQ_EN_SHIFT      =  1,
        READ_BACK_SHIFT   =  0,
    };
    enum CfgBit {
        KEEP_CFG,
        OVERWRITE_CFG,
    };
    enum InChCfgBits {
        BIPOLAR_DIFF_PAIRS  = 0,
        BIPOLAR             = 2,
        TEMP_SENSOR         = 3,
        UNIPOLAR_DIFF_PAIRS = 4,
        UNIPOLAR_REF_TO_COM = 6,
        UNIPOLAR_REF_TO_GND = 7,
    };
    enum BwSelectBit {
        QUARTER_BW,
        FULL_BW
    };
    enum RefSelectionBits {
        INT_REF2_5_AND_TEMP_SENS,   // REF = 2.5 V buffered output.
        INT_REF4_096_AND_TEMP_SENS, // REF = 4.096 V buffered output.
        EXT_REF_AND_TEMP_SENS,      // Internal buffer disabled
        EXT_REF_AND_TEMP_SENS_BUFF, // Internal buffer and temperature sensor enabled.
        EXT_REF = 6,                // Int ref, int buffer, and temp sensor disabled.
        EXT_REF_BUFF,               // Int buffer enabled. Int ref and temp sensor disabled.
    };
    enum ChSeqBits {
        DISABLE_SEQ,
        UPDATE_CFG,
        SCAN_IN_CH_AND_TEMP,
        SCAN_IN_CH,
    };
    enum ReadBackBit {
        READ_BACK_EN,
        READ_BACK_DISABLE,
    };
public:
    LibAD7699(LibMibSpi* libWrapMibSpi,
              uint32 mibSpiGroup,
              LibWrapGioPort::Port* somiSw,
              uint32 somiSelect,
              LibWrapGioPort::Port* adcCnv,
              float vref     = 5.0,
              bool isIntVref = false);
    // LibAdc
    virtual int read(int channel, uint32& value); // 0-65535
    void initialize();
private:
    int set(int channel);
    int convert();
    int get(uint16& data);
private:
    LibMibSpi& m_libWrapMibSpi;
    uint32 m_mibSpiGroup;
    LibWrapGioPort::Port* m_somiSw;
    uint32 m_somiSelect;
    LibWrapGioPort::Port& m_adcCnv;
    bool m_isIntVref;
};
