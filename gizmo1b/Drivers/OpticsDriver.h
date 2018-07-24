#ifndef __OpticsDriver_H
#define __OpticsDriver_H

#include "FreeRTOS.h"
#include "os_task.h"
#include "os_semphr.h"
#include "gio.h"
#include "mibspi.h"
#include "het.h"
#include "rti.h"

#define     kledDacGroup        (0)
#define     kpdAdcGroup         (1)

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
            UNIPOLAR_REF_TO_GND = 7 // INx referenced to GND
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
    /*
     * Use SPI3 and configure SPI3A_SOMI by setting SPI3_SOMI_SW to 0
     *
     * Configure following GPIOs
     * v1 boards:
     * ==========
     * LED GPIO:    N2HET1[13] -> CS   (Output/High);
     *              N2HET1[12] -> LDAC (Output/High)
     * PD ADC GPIO: N2HET1[14] -> CNV  (Output/Low)
     * PD SR GPIO:  N2HET1[24] -> DS   (Serial Data In);
     *              N2HET1[26] -> SHCP (Shift Reg Clock Input)
     *              N2HET1[28] -> STCP (Storage Reg Clock Input)
     * v2 boards
     * =========
     * PD ADC GPIO: N2HET1[26] -> CNV  (Output/Low)
     * PD SR GPIO:  N2HET1[12] -> DS   (Serial Data In/DATA);
     *              N2HET1[13] -> SHCP (Shift Reg Clock Input/CLK)
     *              N2HET1[24] -> STCP (Storage Reg Clock Input/LATCH)
     * PD T CTRL:   N2HET1[28] -> CT_A (Temperature Ctrl A)
     *              N2HET1[30] -> CT_B (Temperature Ctrl B)
     */
    enum OpticsPinMappings {
        OPTICS_MCU_SPI3_SOMI_SW = PIN_CS0,
    };
    enum PhotoDiodePinMappingBoardV1 {
        LED_BOARD_V1_CS_PIN   = PIN_HET_13,
        LED_BOARD_V1_LDAC_PIN = PIN_HET_12,
        PD_BOARD_V1_DATA_PIN  = PIN_HET_24,
        PD_BOARD_V1_CLK_PIN   = PIN_HET_26,
        PD_BOARD_V1_LATCH_PIN = PIN_HET_28,
        PD_BOARD_V1_CNV_PIN   = PIN_HET_14,
    };
    enum PhotodiodePinMappingBoardV2 {
        PD_BOARD_V2_DATA_PIN  = PIN_HET_12,
        PD_BOARD_V2_CLK_PIN   = PIN_HET_13,
        PD_BOARD_V2_LATCH_PIN = PIN_HET_24,
        PD_BOARD_V2_CNV_PIN   = PIN_HET_26,
        PD_BOARD_V2_T_CTRL_A  = PIN_HET_28,
        PD_BOARD_V2_T_CTRL_B  = PIN_HET_30,
    };
    enum LedBoardVersion {
        LED_BOARD_V1 = 1 << 0,
        LED_BOARD_V2 = 1 << 1,
    };
    enum PhotodiodeBoardVersion {
        PHOTODIODE_BOARD_V1 = 1 << 0,
        PHOTODIODE_BOARD_V2 = 1 << 1,
    };
    struct Data {
        uint16_t m_photodiodeResultRaw;
        uint16_t m_photodiodeTemperatureRaw;
    };
    struct BoardVersion {
        int m_photodiodeBoardVersion;
        int m_ledBoardVersion;
    };
public:
    OpticsDriver();
    void GetPhotoDiodeValue(uint32_t nledChanIdx, uint32_t npdChanIdx,
              uint32_t nDuration_us, uint32_t nLedIntensity, struct Data *data);
    void GetPhotoDiodeTemperatureRaw(uint32_t npdChanIdx, struct Data *data);
    void SetBoardVersion(struct BoardVersion& boardVersion);
    void GetBoardVersion(struct BoardVersion& boardVersion);
    float GetPhotodiodeVref();
private:
    void SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity);
    void SetLedsOff(uint32_t nChanIdx);
    void OpticsDriverInit();
    void AdcConfig();
    void SetIntegratorState(pdIntegratorState state, uint32_t npdChanIdx);
    uint16_t GetAdc(uint32_t nChanIdx);
    void SetPhotodiodeTemperatureCtrl(uint32_t npdChanIdx);
private:
    struct BoardVersion m_boardVersion;
    float m_photodiodeVref;
    uint32 m_dataPin;
    uint32 m_clkPin;
    uint32 m_latchPin;
    uint32 m_cnvPin;
    uint32 m_tCtrlA;
    uint32 m_tCtrlB;
};

#endif // __OpticsDriver_H
