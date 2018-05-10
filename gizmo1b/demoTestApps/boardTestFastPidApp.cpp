#include "FreeRTOS.h"
#include "os_task.h"
#include "libRaisePrivilege.h"
#include "libAdc.h"
#include "libTec.h"
#include "libFault.h"
#include "boardTestTec.h"
#include "boardTestFastPidApp.h"

float BoardTestFastPidApp::s_iRef         = 0.0;
float BoardTestFastPidApp::s_propGain     = 1.0;
float BoardTestFastPidApp::s_intGain      = 0.0;
float BoardTestFastPidApp::s_diffGain     = 0.0;
bool  BoardTestFastPidApp::s_isTecEnabled = true;
bool  BoardTestFastPidApp::s_isClosedLoopEnabled;
bool  BoardTestFastPidApp::s_isWaveFormStarted;
float BoardTestFastPidApp::s_iSense;
uint32 BoardTestFastPidApp::s_status;
uint32 BoardTestFastPidApp::s_waveFormType;
uint32 BoardTestFastPidApp::s_waveFormPeriod = 1000;
uint32 BoardTestFastPidApp::s_waveFormTimeInUs;
uint32 BoardTestFastPidApp::s_loopTimeInUs   = 20;
bool  BoardTestFastPidApp::s_isClosedLoopInitialized;
bool  BoardTestFastPidApp::s_isBoardTestFastPidApp;
bool  BoardTestFastPidApp::s_isSciStart;
int   BoardTestFastPidApp::s_sciRxCount;
int   BoardTestFastPidApp::s_sciTxCount;
union BoardTestApp::MasterToSlaveMsg BoardTestFastPidApp::s_masterToSlaveMsg;
union BoardTestApp::SlaveToMasterMsg BoardTestFastPidApp::s_slaveToMasterMsg;

static void launcherTask(void* parameter)
{
    LibRaisePrivilege libRaisePrivilege;
    BoardTestFastPidApp boardTestFastPidApp("BoardTestFastPidApp");
    boardTestFastPidApp.start();
    while (true) {
    }
}

// Interrupts are enabled in the scheduler so application must be started
// AFTER the scheduler because we need interrupts enabled when we run the
// application
void BoardTestFastPidApp::boardTestFastPidApp2()
{
    TaskHandle_t taskHandle;
    xTaskCreate(launcherTask, "Launcher", configMINIMAL_STACK_SIZE, NULL,
                                             tskIDLE_PRIORITY + 1, &taskHandle);
    vTaskStartScheduler();
    while (true) {
    }
}

BoardTestFastPidApp::BoardTestFastPidApp(const char* name) :
    BoardTestApp(name)
{
}

BoardTestFastPidApp::~BoardTestFastPidApp()
{
}

void BoardTestFastPidApp::run()
{
    LibAdc libAdc;
    LibTec libTec;
    LibFault libFault;
    LibDac libDac;
    libTec.enable(true);
    while (1) {
        libFault.reset();
        float value;
        volatile int result = libAdc.read(LibAdc::CHANNEL_1, value);
        result = libDac.set(2.5);
    }
}

void BoardTestFastPidApp::sciNotification(sciBASE_t* sci, uint32 flags)
{
    //
    // debug pin
    //
    gioSetBit(hetPORT1, PIN_HET_12, 1);  // 124:HET1_12:DIG_OUT_A0
    uint8 byte;
    if (flags & SCI_RX_INT) {
        byte = (uint8)(sci->RD & 0x000000FFU);
        if (!s_isSciStart) {
            restartTimer0();
            s_isSciStart = true;
            s_sciRxCount = 0;
            s_masterToSlaveMsg.buffer[s_sciRxCount++] = byte;
        }
        else if (isTimer0Expired(EXPIRED_TIME_IN_US)) {
            s_isSciStart = false;
            s_sciRxCount = 0;
        }
        else {
            s_masterToSlaveMsg.buffer[s_sciRxCount++] = byte;
            if (s_sciRxCount >= sizeof(s_masterToSlaveMsg.buffer)) {
                s_isSciStart = false;
                s_sciRxCount = 0;
                //
                // New data available
                //
                struct MasterToSlaveMsg::msgMasterToSlave& msgMtoS = s_masterToSlaveMsg.msg;
                uint8  command = msgMtoS.command;
                uint32 address = msgMtoS.address[0] << 24
                               | msgMtoS.address[1] << 16
                               | msgMtoS.address[2] << 8
                               | msgMtoS.address[3];
                uint32 value   = msgMtoS.data[0] << 24
                               | msgMtoS.data[1] << 16
                               | msgMtoS.data[2] << 8
                               | msgMtoS.data[3];
                int result = BoardTest::OKAY;
                float v;
                uint32 status = LibTec::OKAY;
                if (command == BoardTest::REG_WRITE) {
                    switch (address) {
                    default:
                        result = BoardTest::ERROR_ADDR;
                        break;
                    case BoardTest::TEC_ISENSE_VALUE:
                    case BoardTest::TEC_STATUS:
                        result = BoardTest::ERROR_RO;
                        break;
                    case BoardTest::TEC_DERIVATIVE_GAIN:
                        v = *reinterpret_cast<float*>(&value);
                        if (0 <= v && v <= 100) {
                            s_diffGain = *reinterpret_cast<float*>(&value);
                            s_isClosedLoopInitialized = false;
                        }
                        else {
                            status = LibTec::ERROR_DERIVATIVE_GAIN_OUT_OF_RANGE;
                        }
                        break;
                    case BoardTest::TEC_INTEGRAL_GAIN:
                        v = *reinterpret_cast<float*>(&value);
                        if (0 <= v && v <= 100) {
                            s_intGain = *reinterpret_cast<float*>(&value);
                            s_isClosedLoopInitialized = false;
                        }
                        else {
                            status = LibTec::ERROR_INTEGRAL_GAIN_OUT_OF_RANGE;
                        }
                        break;
                    case BoardTest::TEC_PROPORTIONAL_GAIN:
                        v = *reinterpret_cast<float*>(&value);
                        if (0.01 <= v && v <= 100) {
                            s_propGain = *reinterpret_cast<float*>(&value);
                            s_isClosedLoopInitialized = false;
                        }
                        else {
                            status = LibTec::ERROR_PROPORTIONAL_GAIN_OUT_OF_RANGE;
                        }
                        break;
                    case BoardTest::TEC_IREF_VALUE:
                        v = *reinterpret_cast<float*>(&value);
                        if (-15 <= v && v <= 15) {
                            s_iRef = *reinterpret_cast<float*>(&value);
                            s_isClosedLoopInitialized = false;
                        }
                        else {
                            status = LibTec::ERROR_REF_CURRENT_OUT_OF_RANGE;
                        }
                        break;
                    case BoardTest::TEC_WAVEFORM_TYPE:
                        switch (value) {
                        default:
                            status = LibTec::ERROR_WAVEFORM_TYPE_OUT_OF_RANGE;
                            break;
                        case LibTec::WAVEFORM_TYPE_CONSTANT:
                        case LibTec::WAVEFORM_TYPE_SQUARE:
                            s_waveFormType = value;
                            break;
                        }
                        s_waveFormType = *reinterpret_cast<float*>(&value);
                        break;
                    case BoardTest::TEC_WAVEFORM_PERIOD:
                        if (2 <= value && value <= 10000) {
                            s_waveFormPeriod = value;
                        }
                        else {
                            status = LibTec::ERROR_WAVEFORM_PERIOD_OUT_OF_RANGE;
                        }
                        break;
                    case BoardTest::TEC_CONTROL:
                        if (value & BoardTestTec::DISABLE) {
                            s_isTecEnabled = false;
                            s_isClosedLoopInitialized = false;
                            gioSetBit(mibspiPORT1, PIN_CS0, 1);  // ERROR_RED_LED <- 1
                            gioSetBit(mibspiPORT5, PIN_SIMO, s_isTecEnabled); // Enable TEC
                        }
                        if (value & BoardTestTec::ENABLE) {
                            s_isTecEnabled = true;
                            s_isClosedLoopInitialized = false;
                            gioSetBit(mibspiPORT1, PIN_CS0, 0);  // ERROR_RED_LED <- 0
                            gioSetBit(mibspiPORT5, PIN_SIMO, s_isTecEnabled); // Enable TEC
                        }
                        if (value & BoardTestTec::START_WAVEFORM) {
                            s_isWaveFormStarted = true;
                            s_waveFormTimeInUs = 0;
                            gioSetBit(mibspiPORT1, PIN_ENA, 0);  // DEBUG_GREEN_LED <- 0
                        }
                        if (value & BoardTestTec::STOP_WAVEFORM) {
                            s_isWaveFormStarted = false;
                            gioSetBit(mibspiPORT1, PIN_ENA, 1);  // DEBUG_GREEN_LED <- 1
                        }
                        if (value & BoardTestTec::CLOSED_LOOP_ENABLE) {
                            s_isClosedLoopEnabled     = true;
                            s_isClosedLoopInitialized = false;
                        }
                        if (value & BoardTestTec::CLOSED_LOOP_DISABLE) {
                            s_isClosedLoopEnabled     = false;
                            s_isClosedLoopInitialized = false;
                        }
                        break;
                    }
                    s_status = status;
                }
                else if (command == BoardTest::REG_READ) {
                    switch (address) {
                    default:
                        result = BoardTest::ERROR_ADDR;
                        break;
                    case BoardTest::TEC_DERIVATIVE_GAIN:
                        value = *reinterpret_cast<uint32*>(&s_diffGain);
                        break;
                    case BoardTest::TEC_INTEGRAL_GAIN:
                        value = *reinterpret_cast<uint32*>(&s_intGain);
                        s_isClosedLoopInitialized = false;
                        break;
                    case BoardTest::TEC_PROPORTIONAL_GAIN:
                        value = *reinterpret_cast<uint32*>(&s_propGain);
                        break;
                    case BoardTest::TEC_IREF_VALUE:
                        value = *reinterpret_cast<uint32*>(&s_iRef);
                        break;
                    case BoardTest::TEC_CONTROL:
                        value  = 0;
                        value |= s_isClosedLoopEnabled ? BoardTestTec::CLOSED_LOOP_ENABLE : BoardTestTec::CLOSED_LOOP_DISABLE;
                        value |= s_isTecEnabled        ? BoardTestTec::ENABLE             : BoardTestTec::DISABLE;
                        value |= s_isWaveFormStarted   ? BoardTestTec::START_WAVEFORM     : BoardTestTec::STOP_WAVEFORM;
                        break;
                    case BoardTest::TEC_ISENSE_VALUE:
                        value = *reinterpret_cast<uint32*>(&s_iSense);
                        break;
                    case BoardTest::TEC_STATUS:
                        value = s_status;
                        break;
                    case BoardTest::TEC_WAVEFORM_TYPE:
                        value = s_waveFormType;
                        break;
                    case BoardTest::TEC_WAVEFORM_PERIOD:
                        value = s_waveFormPeriod;
                        break;
                    }
                }
                else {
                    result = BoardTest::ERROR_CMD;
                }
                struct SlaveToMasterMsg::msgSlaveToMaster& msgStoM = s_slaveToMasterMsg.msg;
                msgStoM.status  = result;
                msgStoM.data[0] = value >> 24;
                msgStoM.data[1] = value >> 16;
                msgStoM.data[2] = value >>  8;
                msgStoM.data[3] = value;
                s_sciTxCount = 0;
                sci->TD     = (uint32)s_slaveToMasterMsg.buffer[s_sciTxCount++];
                sci->SETINT = (uint32)SCI_TX_INT;
            }
        }
    }
    if  (flags & SCI_TX_INT) {
        if (s_sciTxCount < sizeof(s_slaveToMasterMsg.buffer)) {
            sci->TD = (uint32)s_slaveToMasterMsg.buffer[s_sciTxCount++];
        }
        else {
            sci->CLEARINT = (uint32)SCI_TX_INT;
        }
    }
    //
    // debug pin
    //
    gioSetBit(hetPORT1, PIN_HET_12, 0);  // 124:HET1_12:DIG_OUT_A0
}

void BoardTestFastPidApp::boardTestFastPidApp()
{
    //
    // Initialize peripherals
    //
    hetInit();
    gioInit();
    mibspiInit();
    rtiInit();
    sciInit();
    s_isBoardTestFastPidApp = true;
    _enable_IRQ();
    gioSetBit(mibspiPORT5, PIN_SIMO, 0); // Disable TEC
    gioSetBit(mibspiPORT5, PIN_ENA, 0);  // DRV_ERR_CLR <- 0
    gioSetBit(mibspiPORT1, PIN_ENA, s_isWaveFormStarted ? 0 : 1); // 96:MIBSPI1NENA:DEBUG_GREEN_LED
    gioSetBit(mibspiPORT1, PIN_CS0, s_isTecEnabled      ? 0 : 1); // 105:MIBSPI1NCS[0]:ERROR_RED_LED
    //
    // Set DAC output to 2.5
    //
    float dacVoltage = 2.5;
    uint16 dacBuffer[3];
    uint16 dacValue = dacVoltage * (65535 / 5.0);
    // Write to DAC-A input register and update DAC-A
    dacBuffer[0] = CMD_WR_ONE_REG_AND_UPDATE_ONE_DAC << CMD_SHIFT
                | ADDR_DAC_A << ADDR_SHIFT;
    dacBuffer[1] = dacValue >> 8;
    dacBuffer[2] = dacValue;
    gioSetBit(gioPORTA, 6, 0); // SYNC <- 0
    mibspiSetData(mibspiREG1, DAC8563SDGST_16_BIT_DAC, dacBuffer);
    mibspiTransfer(mibspiREG1, DAC8563SDGST_16_BIT_DAC);
    while (!mibspiIsTransferComplete(mibspiREG1, DAC8563SDGST_16_BIT_DAC));
    gioSetBit(gioPORTA, 6, 1); // SYNC <- 1
    //
    // Setup ADC for Isense read
    //
    uint16 adcData;
    uint16 adcCfg;
    adcCfg  = 0;
    adcCfg |= OVERWRITE_CFG       << CFG_SHIFT;
    adcCfg |= UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT;
    adcCfg |= ISENSE              << IN_CH_SEL_SHIFT;
    adcCfg |= FULL_BW             << FULL_BW_SEL_SHIFT;
    adcCfg |= EXT_REF             << REF_SEL_SHIFT;
    adcCfg |= DISABLE_SEQ         << SEQ_EN_SHIFT;
    adcCfg |= READ_BACK_DISABLE   << READ_BACK_SHIFT;
    adcCfg  = adcCfg              << 2;
    gioSetBit(mibspiPORT3, PIN_ENA, 0); //mibspi1 somi select spi A
    mibspiSetData(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC, &adcCfg);
    gioSetBit(mibspiPORT5, PIN_SOMI, 0); // ADC_CNV <- 0
    mibspiTransfer(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC);
    while (!mibspiIsTransferComplete(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC));
    gioSetBit(mibspiPORT5, PIN_SOMI, 1); // ADC_CNV <- 1
    usDelay1(CONV_TIME_IN_US); // allow conv and acq to complete
    adcCfg  = 0;
    mibspiSetData(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC, &adcCfg);
    gioSetBit(mibspiPORT5, PIN_SOMI, 0); // ADC_CNV <- 0
    mibspiTransfer(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC); // clock out CFG, clock in DATA
    while (!mibspiIsTransferComplete(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC));
    gioSetBit(mibspiPORT5, PIN_SOMI, 1); // ADC_CNV <- 1
    usDelay1(CONV_TIME_IN_US); // allow conv and acq to complete
    mibspiSetData(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC, &adcCfg);
    gioSetBit(mibspiPORT5, PIN_SOMI, 0); // ADC_CNV <- 0
    mibspiTransfer(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC);
    while (!mibspiIsTransferComplete(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC));
    gioSetBit(mibspiPORT5, PIN_SOMI, 1); // ADC_CNV <- 1
    usDelay1(CONV_TIME_IN_US); // allow conv and acq to complete
    //
    // Clear TEC errors
    //
    gioSetBit(mibspiPORT5, PIN_ENA, 1);
    usDelay1(1); // >1us
    gioSetBit(mibspiPORT5, PIN_ENA, 0);
    //
    // PID Loop parameters
    //
    float pidPrevError;
    float pidAccError;
    float pidPropGain;
    float pidIntGain;
    float pidDiffGain;
    //
    // debug pin
    //
    gioSetBit(hetPORT1, PIN_HET_12, 1);  // 124:HET1_12:DIG_OUT_A0
    while (true) {
        //
        // Clear TEC errors (2.2us)
        //
        gioSetBit(mibspiPORT5, PIN_ENA, 1);
        usDelay1(1); // >1us
        gioSetBit(mibspiPORT5, PIN_ENA, 0);
        //
        // If enabled, generate the square wave
        //
        if (s_isWaveFormStarted) {
            if (s_waveFormTimeInUs > s_waveFormPeriod * 1000) {
                s_waveFormTimeInUs = 0;
                s_iRef = -s_iRef;
            }
            else if (s_waveFormTimeInUs < s_waveFormPeriod * 500
                  && s_waveFormTimeInUs + s_loopTimeInUs >= s_waveFormPeriod * 500) {
                s_iRef = -s_iRef;
            }
            s_waveFormTimeInUs += s_loopTimeInUs;
        }
        //
        // Read Isense (10.9us)
        //
        mibspiSetData(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC, &adcCfg);
        gioSetBit(mibspiPORT5, PIN_SOMI, 0); // ADC_CNV <- 0
        mibspiTransfer(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC);
        while (!mibspiIsTransferComplete(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC));
        gioSetBit(mibspiPORT5, PIN_SOMI, 1); // ADC_CNV <- 1
        mibspiGetData(mibspiREG1, AD7689ACPZ_8CH_16BIT_ADC, &adcData);
        usDelay1(CONV_TIME_IN_US); // allow conv and acq to complete
        float adcVoltage =  adcData * (5.0 / 65535);
        float iSense     = (adcVoltage * 9.05 / 6.04 - 2.5) / (20.0 * 0.008);
        float iRef       =  s_iRef;
        s_iSense         =  iSense;
        //
        // Apply PID (1us)
        //
        float pidError = iRef;
        if (s_isClosedLoopEnabled) {
            pidError -= iSense;
        }
        if (!s_isClosedLoopInitialized || !s_isTecEnabled) {
            pidPropGain  = s_propGain;
            pidIntGain   = s_intGain;
            pidDiffGain  = s_diffGain;
            pidPrevError = pidError;
            pidAccError  = 0;
            if (!s_isClosedLoopInitialized) {
                s_isClosedLoopInitialized = true;
            }
        }
        if (pidDiffGain == 0) {
            pidPrevError = pidError;
        }
        if (pidIntGain == 0) {
            pidAccError = 0;
        }
        float control = pidPropGain *  pidError
                      + pidIntGain  *  pidAccError
                      + pidDiffGain * (pidError - pidPrevError);
        pidPrevError  = pidError;
        pidAccError  += pidError;
        if (s_isTecEnabled) {
            //
            // Set DAC output (5us)
            //
            dacVoltage = control / 15 * 2.5 + 2.5;
            dacVoltage = dacVoltage > 5 ? 5 : dacVoltage < 0 ? 0 : dacVoltage; // limit to [0V,5V]
            uint16 dacData = dacVoltage * (65535 / 5.0);
            // Write to DAC-A input register and update DAC-A
            dacBuffer[1] = dacData >> 8;
            dacBuffer[2] = dacData;
            gioSetBit(gioPORTA, 6, 0); // SYNC <- 0
            mibspiSetData(mibspiREG1, DAC8563SDGST_16_BIT_DAC, dacBuffer);
            mibspiTransfer(mibspiREG1, DAC8563SDGST_16_BIT_DAC);
            while (!mibspiIsTransferComplete(mibspiREG1, DAC8563SDGST_16_BIT_DAC));
            gioSetBit(gioPORTA, 6, 1); // SYNC <- 1
        }
    }
}
