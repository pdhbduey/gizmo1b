#pragma once

#include <FreeRTOS.h>
#include <os_semphr.h>
#include <libWrapGioPort.h>
#include <LibMibSpi3.h>

class LibAdcLedBoardVersion2
{
public:
    enum AdcChannel {
        ADC_CHANNEL_0,
        ADC_CHANNEL_1,
        ADC_CHANNEL_2,
        ADC_CHANNEL_3,
        ADC_CHANNEL_4,
        ADC_CHANNEL_5,
        ADC_CHANNEL_6,
        ADC_CHANNEL_7,
    };
    enum Status {
        OKAY,
        ERROR_INVALID_CHANNEL,
        ERROR_TIME_OUT,
    };
public:
    LibAdcLedBoardVersion2();
    virtual ~LibAdcLedBoardVersion2();
    int read(int channel, float& value);  // 0-Vref
    int read(int channel, uint32& value); // 0-65535
    float getVref();
private:
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
        INT_REF4_096_AND_TEMP_SENS = 1, // Internal reference and temperature sensor enabled. REF = 4.096 V buffered output.
        EXT_REF_AND_TEMP_SENS,          // Use external reference. Temperature sensor enabled. Internal buffer disabled.
        EXT_REF_AND_TEMP_SENS_BUFF,     // Use external reference. Internal buffer and temperature sensor enabled.
        EXT_REF                    = 6, // Use external reference. Internal reference, internal buffer and temperature sensor disabled.
        EXT_REF_BUFF,                   // Use external reference. Internal buffer enabled. Internal reference and temperature sensor disabled.
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
private:
    bool isChannelCorrect(int channel);
    int set(int channel);
    int convert();
    int get(uint16& data);
private:
    LibMibSpi3 m_libMibSpi3;
    LibWrapGioPort::Port m_adcCnv;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
    float m_refV;
};
