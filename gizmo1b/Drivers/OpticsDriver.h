#ifndef __OpticsDriver_H
#define __OpticsDriver_H

#include <gio.h>
#include <mibspi.h>
#include <het.h>

#define     kledDacGroup        (0)
#define     kpdAdcGroup         (1)
#define     maxLedIntensity     (40000)


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class OpticsDriver
{
public:
    enum    {kBlue1 = 0, kGreen, kRed1, kBrown, kRed2, kBlue2, kNumOptChans};
    enum    {kwrInputRegN = 0, kupdateDACRegN, kwrInputupdateAll, kwrInputupdateN, kpwrDownN, kpwrDownChip, kselectIntRef, kselectExtRef, kNoOp};
    enum AdcCfgBit {KEEP_CFG, OVERWRITE_CFG};
    enum AdcBwSelectBit {QUARTER_BW, FULL_BW};
    enum AdcReadBackBit {READ_BACK_EN, READ_BACK_DISABLE};
    enum AdcRefSelectionBits {
            INT_REF2_5_AND_TEMP_SENS,   // REF = 2.5 V buffered output.
            INT_REF4_096_AND_TEMP_SENS, // REF = 4.096 V buffered output.
            EXT_REF_AND_TEMP_SENS,      // Internal buffer disabled
            EXT_REF_AND_TEMP_SENS_BUFF, // Internal buffer and temperature sensor enabled.
            EXT_REF = 6,                // Int ref, int buffer, and temp sensor disabled.
            EXT_REF_BUFF                // Int buffer enabled. Int ref and temp sensor disabled.
        };
    enum AdcChSeqBits {
            DISABLE_SEQ,
            UPDATE_CFG,
            SCAN_IN_CH_AND_TEMP,
            SCAN_IN_CH
        };
    enum AdcInChCfgBits {
            BIPOLAR_DIFF_PAIRS  = 0, // INx referenced to VREF/2 ± 0.1 V.
            BIPOLAR             = 2, // INx referenced to COM = VREF/2 ± 0.1 V.
            TEMP_SENSOR         = 3, // Temperature sensor
            UNIPOLAR_DIFF_PAIRS = 4, // INx referenced to GND ± 0.1 V.
            UNIPOLAR_REF_TO_COM = 6, // INx referenced to COM = GND ± 0.1 V.
            UNIPOLAR_REF_TO_GND = 7  // INx referenced to GND
         };
    enum PDAdcChannels {
            PDINPUTA1 = 0,
            PDINPUTA2 = 1,
            PDINPUTA3 = 2,
            PDINPUTB1 = 3,
            PDINPUTB2 = 4,
            PDINPUTB3 = 5
        };
    enum AdcCtrlRegisterShifts {
            READ_BACK_SHIFT   =  0,
            SEQ_EN_SHIFT      =  1,
            REF_SEL_SHIFT     =  3,
            FULL_BW_SEL_SHIFT =  6,
            IN_CH_SEL_SHIFT   =  7,
            IN_CH_CFG_SHIFT   = 10,
            CFG_SHIFT         = 13
        };
    enum pdIntegratorState {
        RESET_STATE = 0,
        HOLD_STATE = 1,
        INTEGRATE_STATE = 2
    };
    enum pdIntegratorSwitch {
        RESET_SW = 0,
        HOLD_SW = 1
    };
    enum pdShiftRegisterPins {
        PDSR_DATA_PIN = PIN_HET_12,
        PDSR_CLK_PIN = PIN_HET_13,
        PDSR_LATCH_PIN = PIN_HET_24
    };
    enum ledDacPins {
        LED_LDAC_PIN = PIN_HET_12,
        LED_CS_PIN = PIN_HET_13
    };

    enum pdAdcPins {
        PD_CS_PIN = PIN_HET_26
    };

    enum pdSomiSw {
        SPI_A,
        SPI_B
    };


    OpticsDriver(uint32_t nSiteIdx = 0);
    void SetLedState(uint32_t nChanIdx, bool bStateOn = true);
    void SetLedState2(uint32_t nChanIdx, uint32_t nIntensity, uint32_t nDuration_us);
    virtual void SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity) = 0;
    void SetLedsOff();
    uint32_t GetPhotoDiodeValue(uint32_t nledChanIdx, uint32_t npdChanIdx, uint32_t nDuration_us, uint32_t nLedIntensity);
    void OpticsDriverInit();
    void AdcConfig();
    void SetIntegratorState(pdIntegratorState state, uint32_t npdChanIdx);
    uint16_t GetAdc(uint32_t nChanIdx);

public:
    uint32_t            _pd_cs_pin;
    uint32_t            _pdsr_data_pin;
    uint32_t            _pdsr_clk_pin;
    uint32_t            _pdsr_latch_pin;
    uint32_t            _somisw_pin;
    gioPORT_t*          _somisw_gioport;
    uint32_t            _pd_somisw;
  
private:
    uint32_t            _nLedStateMsk;
};

#endif // __OpticsDriver_H
