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
#define     kledDacAD5683RGroup (2)

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
        UNIPOLAR_REF_TO_GND = 7, // INx referenced to GND
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
        RESET_STATE     = 0,
        HOLD_STATE      = 1,
        INTEGRATE_STATE = 2
    };
    enum pdIntegratorSwitch {
        RESET_SW = 0,
        HOLD_SW  = 1
    };
    /*
     * =========================================================================
     * MCU GPIO: MIBSPI3NCS[0] -> MCU_SPI3_SOMI_SW (0 - SPI3A_SOMI, 1 - SPI3B_SOMI)
     * =========================================================================
     * LED v1 board
     * =========================================================================
     * DAC SPI:  SPI3B_SIMO    -> EXT_DAC_SPI_SIMO
     *           SPI3B_CLK     -> EXT_DAC_SPI_CLK
     *           SPI3B_SOMI    -> EXT_DAC_SPI_SOMI
     * DAC GPIO: N2HET1[13]    -> EXT_DAC_CS/LD     (Output/High);
     *           N2HET1[12]    -> EXT_DAC_LDAC      (Output/High)
     * =========================================================================
     * PD v1 board
     * =========================================================================
     * ADC SPI:  SPI3A_SIMO -> EXT_ADC_SPI1_SIMO
     *           SPI3A_CLK  -> EXT_ADC_SPI1_CLK
     *           SPI3A_SOMI -> EXT_ADC_SPI1_SOMI
     * ADC GPIO: N2HET1[14] -> EXT_ADC_CNV       (Output/Low)
     * SR  GPIO: N2HET1[24] -> EXT_ADC_SHFT_DS   (Serial Data In);
     *           N2HET1[26] -> EXT_ADC_SHFT_SHCP (Shift   Reg Clock Input/CLK)
     *           N2HET1[28] -> EXT_ADC_SHFT_STCP (Storage Reg Clock Input/LATCH)
     * =========================================================================
     * LED v2 board
     * =========================================================================
     * DAC SPI:  SPI3B_SIMO -> LED_DAC_SDI
     *           SPI3B_CLK  -> LED_DAC_SCLK
     * GPIO:     N2HET1[15] -> LED_CTRL_S0   (Output)
     *           N2HET1[13] -> LED_CTRL_S1   (Output)
     *           J13[10]GND -> LED_CTRL_S2   (Output, always 0)
     *           N2HET1[14] -> LED_DAC_SYNC  (Output/Low)
     * ADC SPI:  SPI3A_SIMO -> ADC_SPI1_SIMO
     *           SPI3A_CLK  -> ADC_SPI1_CLK
     *           SPI3A_SOMI -> ADC_SPI1_SOMI
     * ADC GPIO: N2HET1[12] -> ADC_CNV       (Output/Low)
     * =========================================================================
     * PD v2 board
     * =========================================================================
     * ADC SPI:   SPI3A_SIMO -> EXT_ADC_SPI1_SIMO
     *            SPI3A_CLK  -> EXT_ADC_SPI1_CLK
     *            SPI3A_SOMI -> EXT_ADC_SPI1_SOMI
     * ADC GPIO:  N2HET1[26] -> ADC_CNV           (Output/Low)
     * SR  GPIO:  N2HET1[12] -> EXT_ADC_SHFT_DS   (Serial Data In/DATA);
     *            N2HET1[13] -> EXT_ADC_SHFT_SHCP (Shift   Reg Clock Input/CLK)
     *            N2HET1[24] -> EXT_ADC_SHFT_STCP (Storage Reg Clock Input/LATCH)
     * TEMP CTRL: N2HET1[28] -> TEMP_SW_CTRL_A    (Temperature Ctrl A)
     *            N2HET1[30] -> TEMP_SW_CTRL_B    (Temperature Ctrl B)
     * =========================================================================
     */
    enum McuSpi3SomiSw {
        OPTICS_MCU_SPI3_SOMI_SW = PIN_CS0,
    };
    enum McuSpi3SomiSwSelect {
        OPTICS_MCU_SPI3A_SOMI = 0,
        OPTICS_MCU_SPI3B_SOMI = 1,
    };
    enum LedPinMappingBoardV1 {
        LED_BOARD_V1_CS_PIN   = PIN_HET_13,
        LED_BOARD_V1_LDAC_PIN = PIN_HET_12,
    };
    enum PhotoDiodePinMappingBoardV1 {
        PD_BOARD_V1_DATA_PIN  = PIN_HET_24,
        PD_BOARD_V1_CLK_PIN   = PIN_HET_26,
        PD_BOARD_V1_LATCH_PIN = PIN_HET_28,
        PD_BOARD_V1_CNV_PIN   = PIN_HET_14,
    };
    enum LedPinMappingBoardV2 {
        LED_BOARD_V2_ADC_CNV      = PIN_HET_12,
        LED_BOARD_V2_LED_CTRL_S0  = PIN_HET_15,
        LED_BOARD_V2_LED_CTRL_S1  = PIN_HET_13,
      //LED_BOARD_V2_LED_CTRL_S2  = Not on BB
        LED_BOARD_V2_LED_DAC_SYNC = PIN_HET_14,
    };
    enum PhotodiodePinMappingBoardV2 {
        PD_BOARD_V2_DATA_PIN    = PIN_HET_12,
        PD_BOARD_V2_CLK_PIN     = PIN_HET_13,
        PD_BOARD_V2_LATCH_PIN   = PIN_HET_24,
        PD_BOARD_V2_CNV_PIN     = PIN_HET_26,
        PD_BOARD_V2_TEMP_CTRL_A = PIN_HET_28,
        PD_BOARD_V2_TEMP_CTRL_B = PIN_HET_30,
    };
    enum LedBoardVersion {
        LED_BOARD_V1 = 1 << 0,
        LED_BOARD_V2 = 1 << 1,
    };
    enum PhotodiodeBoardVersion {
        PHOTODIODE_BOARD_V1 = 1 << 0,
        PHOTODIODE_BOARD_V2 = 1 << 1,
    };
    enum DacAD5683R {
        DAC_AD5683R_DO_NOTHING,
        DAC_AD5683R_WRITE_INPUT_REGISTER,
        DAC_AD5683R_UPDATE_DAC_REGISTER,
        DAC_AD5683R_WRITE_DAC_AND_INPUT_REGISTER,
        DAC_AD5683R_WRITE_CTRL_REGISTER,
        DAC_AD5683R_READ_INPUT_REGISTER,
    };
    struct Data {
        uint16_t m_photodiodeResultRaw;
        uint16_t m_photodiodeTemperatureRaw;
        uint16_t m_ledMontorPhotodiodeResultRaw;
        uint16_t m_ledTemperatureRaw;
    };
    struct BoardVersion {
        int m_photodiodeBoardVersion;
        bool m_isLedBoardEnabled;
        int m_ledBoardVersion;
        bool m_isPhotodiodeBoardEnabled;
    };
public:
    OpticsDriver();
    void GetPhotoDiodeValue(uint32_t nledChanIdx, uint32_t npdChanIdx,
              uint32_t nDuration_us, uint32_t nLedIntensity, struct Data *data);
    void GetPhotoDiodeTemperatureRaw(uint32_t npdChanIdx, struct Data *data);
    void SetBoardVersion(struct BoardVersion& boardVersion);
    void GetBoardVersion(struct BoardVersion& boardVersion);
    float GetPhotodiodeVref();
    void LedBoardEnable();
    void LedBoardDisable();
    void PhotodiodeBoardEnable();
    void PhotodiodeBoardDisable();
    bool IsLedBoardEnabled();
    bool IsPhotodiodeBoardEnabled();
    void GetLedDataRaw(uint32_t nledChanIdx, struct Data *data);
    float GetLedVref();
private:
    void SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity);
    void SetLedOff(uint32_t nChanIdx);
    void AdcConfig(bool useExternalRef);
    void SetIntegratorState(pdIntegratorState state, uint32_t npdChanIdx);
    uint16_t GetAdc(uint32_t nChanIdx, bool useExternalRef);
    void SetPhotodiodeTemperatureCtrl(uint32_t npdChanIdx);
    void SetLedTemperatureCtrl(uint32_t nledChanIdx);
private:
    struct BoardVersion m_boardVersion;
    float m_photodiodeVref;
    float m_ledVref;
    uint32 m_dataPin;
    uint32 m_clkPin;
    uint32 m_latchPin;
    uint32 m_cnvPin;
    uint32 m_tCtrlA;
    uint32 m_tCtrlB;
};

#endif // __OpticsDriver_H
