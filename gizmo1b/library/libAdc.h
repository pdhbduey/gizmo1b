#ifndef _LIB_ADC_H_
#define _LIB_ADC_H_

#include "FreeRTOS.h"
#include "os_semphr.h"
#include "libWrapMibSpi1.h"
#include "libWrapGioPort.h"

class LibAdc
{
public:
    enum Channel {
        CHANNEL_0,
        CHANNEL_1,
        CHANNEL_2,
        CHANNEL_3,
        CHANNEL_4,
        CHANNEL_5,
    };
    enum Status {
        OKAY,
        ERROR_INVALID_CHANNEL,
        ERROR_TIME_OUT,
    };
public:
    LibAdc();
    virtual ~LibAdc();
    int read(int channel, float& value); // 0-5V
    static void test();
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
        BIPOLAR_DIFF_PAIRS  = 0, // INx− referenced to VREF/2 ± 0.1 V.
        BIPOLAR             = 2, // INx referenced to COM = VREF/2 ± 0.1 V.
        TEMP_SENSOR         = 3, // Temperature sensor
        UNIPOLAR_DIFF_PAIRS = 4, // INx− referenced to GND ± 0.1 V.
        UNIPOLAR_REF_TO_COM = 6, // INx referenced to COM = GND ± 0.1 V.
        UNIPOLAR_REF_TO_GND = 7, // INx referenced to GND
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
private:
    bool isChannelCorrect(int channel);
    bool readDataDuringConversion(uint16 cfg, uint16& data);
private:
    LibWrapMibSpi1 m_libWrapMibSpi1;
    LibWrapGioPort::Port m_adcCnv;
    static bool s_isInitialized;
    static SemaphoreHandle_t s_mutex;
};

#endif // _LIB_ADC_H_
