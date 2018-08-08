/*
 * OpticsDriver.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: z003xk2p
 */

#include "OpticsDriver.h"

/**
 * Name: OpticsDriver
 * Parameters:
 * Returns:
 * Description: Configures SPI bus and Turns off LED/PD
 */
OpticsDriver::OpticsDriver()
{
    /* Initialize LED and PD Board Driver */
    struct BoardVersion boardVersion =
        { .m_photodiodeBoardVersion   = PHOTODIODE_BOARD_V1,
          .m_isLedBoardEnabled        = true,
          .m_ledBoardVersion          = LED_BOARD_V1,
          .m_isPhotodiodeBoardEnabled = true, };
    SetBoardVersion(boardVersion);
}

/**
 * Name: SetLedIntensity()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriver::SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity)
{
    if (m_boardVersion.m_ledBoardVersion == LED_BOARD_V1) {
        // Note that LDAC is not used for v1 LED board
        uint16_t nBitPattern[2] = {0xFF00, 0x0000};
        nBitPattern[0] = (uint16_t)(nBitPattern[0] | (((uint16_t)kwrInputupdateN << 4) | nChanIdx));
        nBitPattern[1] = (uint16_t)nLedIntensity;
        gioSetBit(mibspiPORT3, OPTICS_MCU_SPI3_SOMI_SW, OPTICS_MCU_SPI3B_SOMI);
        gioSetBit(hetPORT1, LED_BOARD_V1_CS_PIN, 0);
        mibspiSetData(mibspiREG3, kledDacGroup, nBitPattern);
        mibspiTransfer(mibspiREG3, kledDacGroup);
        while(!(mibspiIsTransferComplete(mibspiREG3, kledDacGroup)));
        gioSetBit(hetPORT1, LED_BOARD_V1_CS_PIN, 1);
    }
    else if (m_boardVersion.m_ledBoardVersion == LED_BOARD_V2) {
        uint16_t nBitPattern[3];
        nBitPattern[0] = DAC_AD5683R_WRITE_DAC_AND_INPUT_REGISTER << 4
                       | nLedIntensity >> 12;
        nBitPattern[1] = nLedIntensity >> 4;
        nBitPattern[2] = nLedIntensity << 4;
        gioSetBit(mibspiPORT3, OPTICS_MCU_SPI3_SOMI_SW, OPTICS_MCU_SPI3B_SOMI);
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_DAC_SYNC, 0);
        mibspiSetData(mibspiREG3, kledDacAD5683RGroup, nBitPattern);
        mibspiTransfer(mibspiREG3, kledDacAD5683RGroup);
        while(!(mibspiIsTransferComplete(mibspiREG3, kledDacAD5683RGroup)));
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_DAC_SYNC, 1);
    }
}

/**
 * Name: SetLedsOff()
 * Parameters:
 * Returns:
 * Description: Turns off LED by setting intensity to 0.
 */
void OpticsDriver::SetLedOff(uint32_t nChanIdx)
{
    SetLedIntensity(nChanIdx, 0);
}

inline static void delayInUs(uint32 timeInUs)
{
    uint32 ticks    = RTI_UDCP1_CONFIGVALUE * timeInUs;
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

inline static void restartTimer()
{
    uint32 counter  = rtiCOUNTER_BLOCK1;
    rtiREG1->GCTRL &= ~(uint32)((uint32)1U << (counter & 3U)); // Stop counter
    //
    // Clear counter
    //
    rtiREG1->CNT[counter].UCx   = 0x00000000U;
    rtiREG1->CNT[counter].FRCx  = 0x00000000U;
    rtiREG1->CMP[counter].COMPx = 0x00000000U;;
    rtiREG1->GCTRL |= ((uint32)1U << (counter & 3U)); // Start counter
}

inline static void waitForUsTimerToExpire(uint32 timeInUs)
{
    uint32 ticks    = RTI_UDCP1_CONFIGVALUE * timeInUs;
    while (rtiREG1->CMP[rtiCOMPARE1].COMPx < ticks);
}

/**
 * Name: GetPhotoDiodeValue()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriver::GetPhotoDiodeValue(uint32_t nledChanIdx, uint32_t npdChanIdx,
               uint32_t nDuration_us, uint32_t nLedIntensity, struct Data *data)
{
    data->m_photodiodeResultRaw      = 0;
    data->m_photodiodeTemperatureRaw = 0;
    uint32_t adcPhotodiodeChannel;
    uint32_t adcPhotodiodeTemperatureChannel;
    if (m_boardVersion.m_isPhotodiodeBoardEnabled) {
        switch(npdChanIdx)
        {
        default:
            // fall through
        case 1:
            npdChanIdx = 0x0001 << PDINPUTA1;
            adcPhotodiodeChannel = 0;
            adcPhotodiodeTemperatureChannel = 6;
            break;
        case 2:
            npdChanIdx = 0x0001 << PDINPUTB1;
            adcPhotodiodeChannel = 3;
            adcPhotodiodeTemperatureChannel = 7;
            break;
        case 3:
            npdChanIdx = 0x0001 << PDINPUTA2;
            adcPhotodiodeChannel = 1;
            adcPhotodiodeTemperatureChannel = 6;
            break;
        case 4:
            npdChanIdx = 0x0001 << PDINPUTB2;
            adcPhotodiodeChannel = 4;
            adcPhotodiodeTemperatureChannel = 7;
            break;
        case 5:
            npdChanIdx = 0x0001 << PDINPUTA3;
            adcPhotodiodeChannel = 2;
            adcPhotodiodeTemperatureChannel = 6;
            break;
        case 6:
            npdChanIdx = 0x0001 << PDINPUTB3;
            adcPhotodiodeChannel = 5;
            adcPhotodiodeTemperatureChannel = 7;
            break;
        }
        /* Reset Integrator first */
        SetIntegratorState(RESET_STATE, npdChanIdx);
        gioSetBit(hetPORT1, m_latchPin, 1);
    }
    /* 10 ms delay */
    delayInUs(10000);
    if (m_boardVersion.m_isLedBoardEnabled) {
        /* Turn On LED */
        SetLedIntensity(nledChanIdx, nLedIntensity);
    }
    /* Hold for 1 ms time before integrating */
    delayInUs(1000);
    if (m_boardVersion.m_isPhotodiodeBoardEnabled) {
        /* Start Integration */
        SetIntegratorState(INTEGRATE_STATE, npdChanIdx);
        gioSetBit(hetPORT1, m_latchPin, 1);
    }
    /* Start the integrator timer */
    taskENTER_CRITICAL();
    restartTimer();
    /* Wait for integration time to expire */
    delayInUs(nDuration_us);
    if (m_boardVersion.m_isPhotodiodeBoardEnabled) {
        /* Prepare the Hold state but don't latch it yet */
        SetIntegratorState(HOLD_STATE, npdChanIdx);
    }
    /* Wait for integration time to expire */
    waitForUsTimerToExpire(nDuration_us);
    if (m_boardVersion.m_isPhotodiodeBoardEnabled) {
        /* Latch the hold state */
        gioSetBit(hetPORT1, m_latchPin, 1);
    }
    taskEXIT_CRITICAL();
    /* Hold for 1ms time before reading */
    delayInUs(1000);
    if (m_boardVersion.m_isPhotodiodeBoardEnabled) {
        bool useExternalRef = true;
        if (m_boardVersion.m_photodiodeBoardVersion == PHOTODIODE_BOARD_V2) {
            useExternalRef = false;
        }
        /* Read photodiode result */
        data->m_photodiodeResultRaw = GetAdc(adcPhotodiodeChannel, useExternalRef);
        if (m_boardVersion.m_photodiodeBoardVersion == PHOTODIODE_BOARD_V2) {
            /* Read photodiode temperature */
            SetPhotodiodeTemperatureCtrl(npdChanIdx);
            data->m_photodiodeTemperatureRaw = GetAdc(adcPhotodiodeTemperatureChannel, useExternalRef);
        }
    }
    GetLedDataRaw(nledChanIdx, data);
    /* Hold for 1ms time before turning off LED */
    delayInUs(1000);
    if (m_boardVersion.m_isLedBoardEnabled) {
        /* Turn Off LED */
        SetLedOff(nledChanIdx);
    }
    /* Reset Integrator */
    if (m_boardVersion.m_isPhotodiodeBoardEnabled) {
        SetIntegratorState(RESET_STATE, npdChanIdx);
        gioSetBit(hetPORT1, m_latchPin, 1);
    }
}

/**
 * Name: GetPhotoDiodeTemperatureRaw()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriver::GetPhotoDiodeTemperatureRaw(uint32_t npdChanIdx,
                                                              struct Data *data)
{
    data->m_photodiodeTemperatureRaw = 0;
    if (!m_boardVersion.m_isPhotodiodeBoardEnabled
      || m_boardVersion.m_photodiodeBoardVersion == PHOTODIODE_BOARD_V1) {
        return;
    }
    uint32_t adcTemperatureChannel;
    switch(npdChanIdx)
    {
    default:
        // fall through
    case 1:
        adcTemperatureChannel = 6;
        break;
    case 2:
        adcTemperatureChannel = 7;
        break;
    case 3:
        adcTemperatureChannel = 6;
        break;
    case 4:
        adcTemperatureChannel = 7;
        break;
    case 5:
        adcTemperatureChannel = 6;
        break;
    case 6:
        adcTemperatureChannel = 7;
        break;
    }
    SetPhotodiodeTemperatureCtrl(npdChanIdx);
    data->m_photodiodeTemperatureRaw = GetAdc(adcTemperatureChannel, false);
}

void OpticsDriver::GetLedDataRaw(uint32_t nledChanIdx, struct Data *data)
{
    data->m_ledMontorPhotodiodeResultRaw = 0;
    data->m_ledTemperatureRaw            = 0;
    if (!m_boardVersion.m_isLedBoardEnabled
      || m_boardVersion.m_ledBoardVersion == LED_BOARD_V1) {
        return;
    }
    /* Read LED monitor photodiode result */
    data->m_ledMontorPhotodiodeResultRaw = GetAdc(6, false);
    /* Read LED temperature */
    SetLedTemperatureCtrl(nledChanIdx);
    data->m_ledTemperatureRaw = GetAdc(nledChanIdx, false);
}

void OpticsDriver::AdcConfig(bool useExternalRef)
{
    uint16_t *adcConfigPointer = NULL;
    uint16_t adcConfig = 0x0000;

    adcConfigPointer = &adcConfig;

    adcConfig |= OVERWRITE_CFG << CFG_SHIFT;
    adcConfig |= UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT;
    adcConfig |= PDINPUTA1 << IN_CH_SEL_SHIFT;
    adcConfig |= FULL_BW << FULL_BW_SEL_SHIFT;
    if (useExternalRef) {
        adcConfig |= EXT_REF << REF_SEL_SHIFT;
    }
    else {
        adcConfig |= INT_REF4_096_AND_TEMP_SENS << REF_SEL_SHIFT;
    }
    adcConfig |= DISABLE_SEQ << SEQ_EN_SHIFT;
    adcConfig |= READ_BACK_DISABLE << READ_BACK_SHIFT;
    adcConfig <<= 2;

    gioSetBit(mibspiPORT3, OPTICS_MCU_SPI3_SOMI_SW, OPTICS_MCU_SPI3A_SOMI);

    /* Send 2 dummy conversion to update config register on Photo Diode ADC */
    /* First dummy conversion */
    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, m_cnvPin, 0); //Start sending command
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, m_cnvPin, 1);
    for(int i=0; i<1000; i++); //Wait for sometime for conv/acq to complete

    /* Wait for conversion to complete ~ 3.2 uS */
    adcConfig = 0;
    /* Second dummy conversion */
    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, m_cnvPin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, m_cnvPin, 1);
    for(int i=0; i<1000; i++);

    /* Set Configuration Value */
    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, m_cnvPin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, m_cnvPin, 1);
    for(int i=0; i<1000; i++);
}

/**
 * Name: GetAdc()
 * Parameters: uint32_t nChanIdx: ADC channel to read
 * Returns: uint16_t nAdcVal: ADC value
 * Description: Obtain value from ADC
 */
uint16_t OpticsDriver::GetAdc(uint32_t nChanIdx, bool useExternalRef)
{
    uint16_t nAdcVal = 0;
    uint16_t *data = NULL;
    uint16_t *adcConfigPointer = NULL;
    uint16_t adcConfig = 0x0000;

    data = &nAdcVal;
    adcConfigPointer = &adcConfig;

    /* Get Adc Value */
    adcConfig |= OVERWRITE_CFG << CFG_SHIFT;
    adcConfig |= UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT;
    adcConfig |= (uint16_t) nChanIdx << IN_CH_SEL_SHIFT;
    adcConfig |= FULL_BW << FULL_BW_SEL_SHIFT;
    if (useExternalRef) {
        adcConfig |= EXT_REF << REF_SEL_SHIFT;
    }
    else {
        adcConfig |= INT_REF4_096_AND_TEMP_SENS << REF_SEL_SHIFT;
    }
    adcConfig |= DISABLE_SEQ << SEQ_EN_SHIFT;
    adcConfig |= READ_BACK_DISABLE << READ_BACK_SHIFT;
    adcConfig <<= 2;

    // Select SPI3A_SOMI
    gioSetBit(mibspiPORT3, OPTICS_MCU_SPI3_SOMI_SW, OPTICS_MCU_SPI3A_SOMI);

    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, m_cnvPin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, m_cnvPin, 1);
    mibspiGetData(mibspiREG3, kpdAdcGroup, data); //Get ADC Value
    for(int i=0; i<1000; i++);

    adcConfig = 0;

    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, m_cnvPin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, m_cnvPin, 1);
    mibspiGetData(mibspiREG3, kpdAdcGroup, data); //Get ADC Value
    for(int i=0; i<1000; i++);

    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, m_cnvPin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, m_cnvPin, 1);
    mibspiGetData(mibspiREG3, kpdAdcGroup, data); //Get ADC Value
    for(int i=0; i<1000; i++);

    nAdcVal = *data;
    return nAdcVal;
}

/**
 * Name: SetIntegratorState()
 * Parameters:
 * Returns:
 * Description: Utilizes shift register to set integrator state
 */
void OpticsDriver::SetIntegratorState(pdIntegratorState state, uint32_t npdChanIdx)
{
    uint8_t pinState = 0;
    uint16_t serialDataIn = 0;
    /* Initialize GPIOs for Shift Register: Pin */
    /* Pull-down Latch pin, Clk pin and Data pin */
    gioSetBit(hetPORT1, m_latchPin, 0);
    gioSetBit(hetPORT1, m_dataPin, 0);

    switch (state)
    {
        case RESET_STATE:
            serialDataIn = 0x0000;
            break;
        case HOLD_STATE:
            serialDataIn = 0xFFFF;
            break;
        case INTEGRATE_STATE:
            serialDataIn = 0x00FF;
            break;
        default:
            break;
    }

    /* shift data into shift register */
    for (int i = 15; i>=0; i--)
    {
        gioSetBit(hetPORT1, m_clkPin, 0);
        /* Checks if serial data is 1 or 0 shifts accordingly */
        if (serialDataIn & (1<<i))
        {
            pinState = 1;
        }
        else
        {
            pinState = 0;
        }
        gioSetBit(hetPORT1, m_dataPin, pinState);
        gioSetBit(hetPORT1, m_clkPin,  1);
        gioSetBit(hetPORT1, m_dataPin, 0);
    }
    gioSetBit(hetPORT1, m_clkPin, 0);
}

void OpticsDriver::SetBoardVersion(struct BoardVersion& boardVersion)
{
    m_boardVersion = boardVersion;
    if (m_boardVersion.m_isLedBoardEnabled) {
        switch (boardVersion.m_ledBoardVersion) {
        default:
        case LED_BOARD_V1:
            m_ledVref = 5;
            gioSetBit(hetPORT1, LED_BOARD_V1_CS_PIN,   1);
            gioSetBit(hetPORT1, LED_BOARD_V1_LDAC_PIN, 1);
            break;
        case LED_BOARD_V2:
            m_ledVref = 4.096;
            m_cnvPin  = LED_BOARD_V2_ADC_CNV;
            gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S0,  0);
            gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S1,  0);
          //gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S2,  0); // not on BB
            gioSetBit(hetPORT1, m_cnvPin,                  1);
            gioSetBit(hetPORT1, LED_BOARD_V2_LED_DAC_SYNC, 1);
            AdcConfig(false);
            {
                // Configure DAC
                uint16_t nBitPattern[3];
                uint32 ctrlRegister = 1 << 15; // enable 2 × VREF output range
                nBitPattern[0] = DAC_AD5683R_WRITE_CTRL_REGISTER << 4
                               | ctrlRegister >> 16;
                nBitPattern[1] = ctrlRegister >> 8;
                nBitPattern[2] = ctrlRegister;
                gioSetBit(mibspiPORT3, OPTICS_MCU_SPI3_SOMI_SW, OPTICS_MCU_SPI3B_SOMI);
                gioSetBit(hetPORT1, LED_BOARD_V2_LED_DAC_SYNC, 0);
                mibspiSetData(mibspiREG3, kledDacAD5683RGroup, nBitPattern);
                mibspiTransfer(mibspiREG3, kledDacAD5683RGroup);
                while(!(mibspiIsTransferComplete(mibspiREG3, kledDacAD5683RGroup)));
                gioSetBit(hetPORT1, LED_BOARD_V2_LED_DAC_SYNC, 1);
            }
            break;
        }
    }
    if (m_boardVersion.m_isPhotodiodeBoardEnabled) {
        switch (boardVersion.m_photodiodeBoardVersion) {
        default:
        case PHOTODIODE_BOARD_V1:
            m_photodiodeVref = 5;
            m_dataPin        = PD_BOARD_V1_DATA_PIN;
            m_clkPin         = PD_BOARD_V1_CLK_PIN;
            m_latchPin       = PD_BOARD_V1_LATCH_PIN;
            m_cnvPin         = PD_BOARD_V1_CNV_PIN;
            gioSetBit(hetPORT1, m_cnvPin,   1);
            gioSetBit(hetPORT1, m_dataPin,  0);
            gioSetBit(hetPORT1, m_clkPin,   0);
            gioSetBit(hetPORT1, m_latchPin, 1); //Latch pin is high to start with
            AdcConfig(true);
            break;
        case PHOTODIODE_BOARD_V2:
            m_photodiodeVref = 4.096;
            m_dataPin        = PD_BOARD_V2_DATA_PIN;
            m_clkPin         = PD_BOARD_V2_CLK_PIN;
            m_latchPin       = PD_BOARD_V2_LATCH_PIN;
            m_cnvPin         = PD_BOARD_V2_CNV_PIN;
            m_tCtrlA         = PD_BOARD_V2_TEMP_CTRL_A;
            m_tCtrlB         = PD_BOARD_V2_TEMP_CTRL_B;
            gioSetBit(hetPORT1, m_cnvPin,   1);
            gioSetBit(hetPORT1, m_dataPin,  0);
            gioSetBit(hetPORT1, m_clkPin,   0);
            gioSetBit(hetPORT1, m_latchPin, 1); //Latch pin is high to start with
            gioSetBit(hetPORT1, m_tCtrlA,   0);
            gioSetBit(hetPORT1, m_tCtrlB,   0);
            AdcConfig(false);
            break;
        }
    }
}

void OpticsDriver::GetBoardVersion(struct BoardVersion& boardVersion)
{
    boardVersion = m_boardVersion;
}

float OpticsDriver::GetPhotodiodeVref()
{
    return m_photodiodeVref;
}

void OpticsDriver::SetPhotodiodeTemperatureCtrl(uint32_t npdChanIdx)
{
    switch(npdChanIdx)
    {
    default:
        // fall through
    case 1:
    case 2:
        gioSetBit(hetPORT1, m_tCtrlA, 0);
        gioSetBit(hetPORT1, m_tCtrlB, 0);
        break;
    case 3:
    case 4:
        gioSetBit(hetPORT1, m_tCtrlA, 0);
        gioSetBit(hetPORT1, m_tCtrlB, 1);
        break;
    case 5:
    case 6:
        gioSetBit(hetPORT1, m_tCtrlA, 1);
        gioSetBit(hetPORT1, m_tCtrlB, 0);
        break;
    }
}

void OpticsDriver::SetLedTemperatureCtrl(uint32_t nledChanIdx)
{
    switch(nledChanIdx)
    {
    default:
        // fall through
    case 0:
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S0, 0);
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S1, 0);
      //gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S2, 0); // not on BB
        break;
    case 1:
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S0, 1);
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S1, 0);
      //gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S2, 0); // not on BB
        break;
    case 2:
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S0, 0);
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S1, 1);
      //gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S2, 0); // not on BB
        break;
    case 3:
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S0, 1);
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S1, 1);
      //gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S2, 0); // not on BB
        break;
    case 4:
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S0, 0);
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S1, 0);
      //gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S2, 1); // not on BB
        break;
    case 5:
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S0, 1);
        gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S1, 0);
      //gioSetBit(hetPORT1, LED_BOARD_V2_LED_CTRL_S2, 1); // not on BB
        break;
    }
}

void OpticsDriver::LedBoardEnable()
{
    struct BoardVersion boardVersion = m_boardVersion;
    boardVersion.m_isLedBoardEnabled = true;
    SetBoardVersion(boardVersion);
}

void OpticsDriver::LedBoardDisable()
{
    struct BoardVersion boardVersion = m_boardVersion;
    boardVersion.m_isLedBoardEnabled = false;
    SetBoardVersion(boardVersion);
}

void OpticsDriver::PhotodiodeBoardEnable()
{
    struct BoardVersion boardVersion        = m_boardVersion;
    boardVersion.m_isPhotodiodeBoardEnabled = true;
    SetBoardVersion(boardVersion);
}

void OpticsDriver::PhotodiodeBoardDisable()
{
    struct BoardVersion boardVersion        = m_boardVersion;
    boardVersion.m_isPhotodiodeBoardEnabled = false;
    SetBoardVersion(boardVersion);
}

bool OpticsDriver::IsLedBoardEnabled()
{
    return m_boardVersion.m_isLedBoardEnabled;
}

bool OpticsDriver::IsPhotodiodeBoardEnabled()
{
    return m_boardVersion.m_isPhotodiodeBoardEnabled;
}

float OpticsDriver::GetLedVref()
{
    return m_ledVref;
}
