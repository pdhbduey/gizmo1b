#ifndef _BOARD_TEST_FAST_PID_APP_H_
#define _BOARD_TEST_FAST_PID_APP_H_

#include "sys_common.h"
#include "het.h"
#include "gio.h"
#include "mibspi.h"
#include "rti.h"
#include "sci.h"
#include "boardTestApp.h"

class BoardTestFastPidApp: public BoardTestApp {
public:
    enum DacCmd {
        CMD_WR_REG_OR_GAINS                = 0, // Write to input register n or to gains
        CMD_UPDATE_ONE_DAC                 = 1, // Software LDAC, update DAC register n
        CMD_WR_ONE_REG_AND_UPDATE_ALL_DACS = 2, // Write to input register n and update all DAC registers
        CMD_WR_ONE_REG_AND_UPDATE_ONE_DAC  = 3, // Write to input register n and update DAC register n
        CMD_POWER_DAC                      = 4, // Set DAC power up or -down mode
        CMD_RESET                          = 5, // Software reset
        CMD_SET_LDAC_PIN                   = 6, // Set LDAC registers
        CMD_ENABLE_INT_REF                 = 7, // Enable or disable the internal reference
    };
    enum DacAddrShift {
        ADDR_SHIFT = 0,
    };
    enum DacCmdShift {
        CMD_SHIFT = 3,
    };
    enum DacAddr {
        ADDR_DAC_A       = 0, // DAC-A input register
        ADDR_DAC_B       = 1, // DAC-B input register
        ADDR_DAC_GAIN    = 2,
        ADDR_DAC_A_AND_B = 7, // DAC-A and DAC-B input registers
    };
    enum MibSpiGroup {
        GROUP_0,
        GROUP_1,
        GROUP_2,
        GROUP_3,
        GROUP_4,
        GROUP_5,
        GROUP_6,
        GROUP_7,

        LOOP_BACK_TEST = GROUP_0,
    };
    enum MibSpi1Group {
        DAC8563SDGST_16_BIT_DAC       = GROUP_1,
        AD7689ACPZ_8CH_16BIT_ADC      = GROUP_2,
        L6470HTR_STEPPER_MOTOR_DRIVER = GROUP_3,
    };
    enum AdcCfgBit {
        KEEP_CFG,
        OVERWRITE_CFG,
    };
    enum AdcInChCfgBits {
        BIPOLAR_DIFF_PAIRS  = 0, // INx− referenced to VREF/2 ± 0.1 V.
        BIPOLAR             = 2, // INx referenced to COM = VREF/2 ± 0.1 V.
        TEMP_SENSOR         = 3, // Temperature sensor
        UNIPOLAR_DIFF_PAIRS = 4, // INx− referenced to GND ± 0.1 V.
        UNIPOLAR_REF_TO_COM = 6, // INx referenced to COM = GND ± 0.1 V.
        UNIPOLAR_REF_TO_GND = 7, // INx referenced to GND
    };
    enum AdcChannels {
        CHANNEL_0,
        CHANNEL_1,
        CHANNEL_2,
        CHANNEL_3,
        CHANNEL_4,
        CHANNEL_5,
        ISENSE     = CHANNEL_1,
        VSENSE     = CHANNEL_0,
        TEMP_AIN_A = CHANNEL_2,
        TEMP_AIN_B = CHANNEL_3,
        TEMP_AIN_C = CHANNEL_4,
        TEMP_AIN_D = CHANNEL_5,
    };
    enum AdcCtrlRegisterShifts {
        CFG_SHIFT         = 13,
        IN_CH_CFG_SHIFT   = 10,
        IN_CH_SEL_SHIFT   =  7,
        FULL_BW_SEL_SHIFT =  6,
        REF_SEL_SHIFT     =  3,
        SEQ_EN_SHIFT      =  1,
        READ_BACK_SHIFT   =  0,
    };
    enum AdcBwSelectBit {
        QUARTER_BW,
        FULL_BW
    };
    enum AdcRefSelectionBits {
        INT_REF2_5_AND_TEMP_SENS,   // REF = 2.5 V buffered output.
        INT_REF4_096_AND_TEMP_SENS, // REF = 4.096 V buffered output.
        EXT_REF_AND_TEMP_SENS,      // Internal buffer disabled
        EXT_REF_AND_TEMP_SENS_BUFF, // Internal buffer and temperature sensor enabled.
        EXT_REF = 6,                // Int ref, int buffer, and temp sensor disabled.
        EXT_REF_BUFF,               // Int buffer enabled. Int ref and temp sensor disabled.
    };
    enum AdcChSeqBits {
        DISABLE_SEQ,
        UPDATE_CFG,
        SCAN_IN_CH_AND_TEMP,
        SCAN_IN_CH,
    };
    enum AdcReadBackBit {
        READ_BACK_EN,
        READ_BACK_DISABLE,
    };
    enum AdcConvTimeInUs {
        CONV_TIME_IN_US = 4,
    };
    enum SciExpiredTimeInUs {
        EXPIRED_TIME_IN_US = 1000,
    };
    inline static void restartTimer0()
    {
        uint32 counter  = rtiCOUNTER_BLOCK0;
        rtiREG1->GCTRL &= ~(uint32)((uint32)1U << (counter & 3U)); // Stop counter
        //
        // Clear counter
        //
        rtiREG1->CNT[counter].UCx   = 0x00000000U;
        rtiREG1->CNT[counter].FRCx  = 0x00000000U;
        rtiREG1->CMP[counter].COMPx = 0x00000000U;;
        rtiREG1->GCTRL |= ((uint32)1U << (counter & 3U)); // Start counter
    }
    inline static bool isTimer0Expired(uint32 usTimeOut)
    {
        uint32 ticks    = RTI_UDCP0_CONFIGVALUE * usTimeOut;
        return rtiREG1->CMP[rtiCOMPARE0].COMPx >= ticks;
    }
    inline static void usDelay1(uint32 usDelay)
    {
        uint32 ticks    = RTI_UDCP1_CONFIGVALUE * usDelay;
        uint32 counter  = rtiCOUNTER_BLOCK1;
        rtiREG1->GCTRL &= ~(uint32)((uint32)1U << (counter & 3U)); // Stop counter
        //
        // Clear counter
        //
        rtiREG1->CNT[counter].UCx   = 0x00000000U;
        rtiREG1->CNT[counter].FRCx  = 0x00000000U;
        rtiREG1->CMP[counter].COMPx = 0x00000000U;;
        rtiREG1->GCTRL |= ((uint32)1U << (counter & 3U)); // Start counter
        while (rtiREG1->CMP[rtiCOMPARE1].COMPx < ticks);
    }
public:
    BoardTestFastPidApp(const char* name);
    virtual ~BoardTestFastPidApp();
    static void boardTestFastPidApp();
    static void boardTestFastPidApp2();
    static void sciNotification(sciBASE_t* sci, uint32 flags);
public:
    static bool  s_isBoardTestFastPidApp;
private:
    static float s_iRef;
    static float s_propGain;
    static float s_intGain;
    static float s_diffGain;
    static bool  s_isClosedLoopInitialized;
    static bool  s_isClosedLoopEnabled;
    static bool  s_isTecEnabled;
    static bool  s_isWaveFormStarted;
    static float s_iSense;
    static uint32 s_status;
    static uint32 s_waveFormType;
    static uint32 s_waveFormPeriod;
    static uint32 s_waveFormTimeInUs;
    static uint32 s_loopTimeInUs;
    static bool  s_isSciStart;
    static int   s_sciRxCount;
    static int   s_sciTxCount;
    static union MasterToSlaveMsg s_masterToSlaveMsg;
    static union SlaveToMasterMsg s_slaveToMasterMsg;
private:
    virtual void run();
};

#endif // _BOARD_TEST_FAST_PID_APP_H_
