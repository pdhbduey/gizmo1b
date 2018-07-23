/*
 * OpticsDriver.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: z003xk2p
 */

#include "OpticsDriver.h"
#include "het.h"

/**
 * Name: OpticsDriver
 * Parameters:
 * Returns:
 * Description: Configures SPI bus and Turns off LED/PD
 */
OpticsDriver::OpticsDriver(uint32_t nSiteIdx)
{
    /* Initialize LED and PD Board Driver */
    OpticsDriverInit();
    struct Data data;
    GetPhotoDiodeValue(0, 1, 100000, 40000, &data);
}

/**
 * Name: GetDarkReading()
 * Parameters:
 * Returns:
 * Description:
 */
uint32_t OpticsDriver::GetDarkReading(uint32_t nChanIdx)
{
    SetLedsOff(nChanIdx);

    //Get average of multiple readings.
    uint32_t nAve = 0;
    for (int i = 0; i < 10; i++)
        nAve += GetAdc(nChanIdx);

    return (nAve / 10);
}

/**
 * Name: GetIlluminatedReading()
 * Parameters:
 * Returns:
 * Description:
 */
uint32_t OpticsDriver::GetIlluminatedReading(uint32_t nChanIdx)
{
    //LED = on, wait for exposure time.
    SetLedState(nChanIdx, true);
    vTaskDelay(2);

    //Get average of multiple readings.
    uint32_t nAve = 0;
    for (int i = 0; i < 10; i++)
        nAve += GetAdc(nChanIdx);

    SetLedsOff(nChanIdx);
    return (nAve / 10);
}

/**
 * Name: SetLedState()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriver::SetLedState(uint32_t nChanIdx, bool bStateOn)
{
    /*        SetLedsOff();
            for (int i = 0; i < 8; i++)
            {
                GPIO_PinState nState = GPIO_PIN_RESET;
                if (bStateOn && ((7 - i) ==  nChanIdx))
                    nState = GPIO_PIN_SET;

                HAL_GPIO_WritePin(kLedSDI_Port, kLedSDI_Pin, nState);
                HAL_GPIO_WritePin(kLedSDI_Port, kLedSDI_Pin, nState);
                HAL_GPIO_WritePin(kLedCLK_Port, kLedCLK_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(kLedCLK_Port, kLedCLK_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(kLedCLK_Port, kLedCLK_Pin, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(kLedCLK_Port, kLedCLK_Pin, GPIO_PIN_RESET);
            }

            //Pulse latch enable and enable outputs.
            HAL_GPIO_WritePin(kLedSDI_Port, kLedSDI_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(kLedLE_Port, kLedLE_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(kLedLE_Port, kLedLE_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(kLedLE_Port, kLedLE_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(kLedLE_Port, kLedLE_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(kLedLE_Port, kLedLE_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(kLedLE_Port, kLedLE_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(kLedOE_Port, kLedOE_Pin, GPIO_PIN_RESET);*/
}

/**
 * Name: SetLedState2()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriver::SetLedState2(uint32_t nChanIdx, uint32_t nIntensity, uint32_t nDuration_us)
{
    SetLedIntensity(nChanIdx, nIntensity);

    //If the user wants to energize LED for a specified amount of time.
    if (nDuration_us > 0)
    {
        for (int i = 0; i < (int)nDuration_us; i++);
        SetLedIntensity(nChanIdx, 0);
    }
}

/**
 * Name: SetLedIntensity()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriver::SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity)
{
    uint16_t nBitPattern[2] = {0xFF00, 0x0000};

    nBitPattern[0] = (uint16_t)(nBitPattern[0] | (((uint16_t)kwrInputupdateN << 4) | nChanIdx));
    nBitPattern[1] = (uint16_t)nLedIntensity;
    gioSetBit(hetPORT1, 13, 0);
    mibspiSetData(mibspiREG3, kledDacGroup, nBitPattern);
    mibspiTransfer(mibspiREG3, kledDacGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kledDacGroup)));
    gioSetBit(hetPORT1, 13, 1);
}

/**
 * Name: SetLedsOff()
 * Parameters:
 * Returns:
 * Description: Turns off LED by setting intensity to 0.
 */
void OpticsDriver::SetLedsOff(uint32_t nChanIdx)
{
    /*uint16_t nBitPattern[2] = {0xFF00, 0xFFFF};


    nBitPattern[0] = (uint16_t)(nBitPattern[0] | ((uint16_t)kpwrDownChip << 4));

    gioSetBit(hetPORT1, 13, 0);
    mibspiSetData(mibspiREG3, kledDacGroup, nBitPattern);
    mibspiTransfer(mibspiREG3, kledDacGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kledDacGroup)));
    gioSetBit(hetPORT1, 13, 1); */

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
    uint16_t adcValue = 0x0000;
    uint32_t adcChannel = 0;
    uint32_t adcTemperatureChannel = 6;

    switch(npdChanIdx)
    {
    default:
        // fall through
    case 1:
        npdChanIdx = 0x0001 << PDINPUTA1;
        adcChannel = 0;
        adcTemperatureChannel = 6;
        // Select temperature input
        // TEMP_SW_CTRL_A = 0
        // TEMP_SW_CTRL_B = 0
        break;
    case 2:
        npdChanIdx = 0x0001 << PDINPUTB1;
        adcChannel = 3;
        adcTemperatureChannel = 7;
        // Select temperature input
        // TEMP_SW_CTRL_A = 0
        // TEMP_SW_CTRL_B = 0
        break;
    case 3:
        npdChanIdx = 0x0001 << PDINPUTA2;
        adcChannel = 1;
        adcTemperatureChannel = 6;
        // Select temperature input
        // TEMP_SW_CTRL_A = 0
        // TEMP_SW_CTRL_B = 1
        break;
    case 4:
        npdChanIdx = 0x0001 << PDINPUTB2;
        adcChannel = 4;
        adcTemperatureChannel = 7;
        // Select temperature input
        // TEMP_SW_CTRL_A = 0
        // TEMP_SW_CTRL_B = 1
        break;
    case 5:
        npdChanIdx = 0x0001 << PDINPUTA3;
        adcChannel = 2;
        adcTemperatureChannel = 6;
        // Select temperature input
        // TEMP_SW_CTRL_A = 1
        // TEMP_SW_CTRL_B = 0
        break;
    case 6:
        npdChanIdx = 0x0001 << PDINPUTB3;
        adcChannel = 5;
        adcTemperatureChannel = 7;
        // Select temperature input
        // TEMP_SW_CTRL_A = 1
        // TEMP_SW_CTRL_B = 0
        break;
    }

    /* Reset Integrator first */
    SetIntegratorState(RESET_STATE, npdChanIdx);
    gioSetBit(hetPORT1, LATCH_PIN, 1);

    /* 10 ms delay */
    delayInUs(10000);

    /* Turn On LED */
    SetLedIntensity(nledChanIdx, nLedIntensity);

    /* Hold for 1 ms time before integrating */
    delayInUs(1000);

    /* Start Integration */
    SetIntegratorState(INTEGRATE_STATE, npdChanIdx);
    gioSetBit(hetPORT1, LATCH_PIN, 1);

    /* Start the integrator timer */
    taskENTER_CRITICAL();
    restartTimer();

    /* Wait for integration time to expire */
    delayInUs(nDuration_us);

    /* Prepare the Hold state but don't latch it yet */
    SetIntegratorState(HOLD_STATE, npdChanIdx);

    /* Wait for integration time to expire */
    waitForUsTimerToExpire(nDuration_us);

    /* Latch the hold state */
    gioSetBit(hetPORT1, LATCH_PIN, 1);
    taskEXIT_CRITICAL();

    /* Hold for 1ms time before reading */
    delayInUs(1000);

    /* Read photodiode result */
    adcValue = GetAdc(adcChannel);
    data->m_photodiodeResultRaw = adcValue;

    if (data->m_photodiodeBoardVersion == PHOTODIODE_BOARD_V2) {
        /* Read photodiode temperature */
        adcValue = GetAdc(adcTemperatureChannel);
        data->m_photodiodeTemperatureRaw = adcValue;
    }
    else {
        data->m_photodiodeTemperatureRaw = 0;
    }

    /* Hold for 1ms time before turning off LED */
    delayInUs(1000);

    /* Turn Off LED */
    SetLedsOff(nledChanIdx);

    /* Reset Integrator */
    SetIntegratorState(RESET_STATE, npdChanIdx);
    gioSetBit(hetPORT1, LATCH_PIN, 1);
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
    if (data->m_photodiodeBoardVersion == PHOTODIODE_BOARD_V1) {
        data->m_photodiodeTemperatureRaw = 0;
        return;
    }
    uint32_t adcTemperatureChannel;
    switch(npdChanIdx)
    {
    default:
        // fall through
    case 1:
        adcTemperatureChannel = 6;
        // Select temperature input
        // TEMP_SW_CTRL_A = 0
        // TEMP_SW_CTRL_B = 0
        break;
    case 2:
        adcTemperatureChannel = 7;
        // Select temperature input
        // TEMP_SW_CTRL_A = 0
        // TEMP_SW_CTRL_B = 0
        break;
    case 3:
        adcTemperatureChannel = 6;
        // Select temperature input
        // TEMP_SW_CTRL_A = 0
        // TEMP_SW_CTRL_B = 1
        break;
    case 4:
        adcTemperatureChannel = 7;
        // Select temperature input
        // TEMP_SW_CTRL_A = 0
        // TEMP_SW_CTRL_B = 1
        break;
    case 5:
        adcTemperatureChannel = 6;
        // Select temperature input
        // TEMP_SW_CTRL_A = 1
        // TEMP_SW_CTRL_B = 0
        break;
    case 6:
        adcTemperatureChannel = 7;
        // Select temperature input
        // TEMP_SW_CTRL_A = 1
        // TEMP_SW_CTRL_B = 0
        break;
    }
    data->m_photodiodeTemperatureRaw = GetAdc(adcTemperatureChannel);
}


/**
 * Name: OpticsDriverInit()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriver::OpticsDriverInit(void)
{
    /* Configure following GPIOs
     * LED GPIO: N2HET1[13] -> CS (Output/High); N2HET1[12] -> LDAC (Output/High)
     * PD ADC GPIO: N2HET1[14] -> CNV (Output/Low)
     * PD SR GPIO: N2HET1[24] -> DS (Serial Data In); N2HET1[26] -> SHCP (Shift Reg Clock Input)
     *             N2HET1[28] -> STCP (Storage Reg Clock Input)
     */
    uint32_t gpioDirectionConfig = 0x00000000;
    uint32_t gpioOutputState = 0x00000000;

    /* Set GPIO pin direction */
    gpioDirectionConfig |= (1<<PIN_HET_12);
    gpioDirectionConfig |= (1<<PIN_HET_13);
    gpioDirectionConfig |= (1<<PIN_HET_14);
    gpioDirectionConfig |= (1<<PIN_HET_24);
    gpioDirectionConfig |= (1<<PIN_HET_26);
    gpioDirectionConfig |= (1<<PIN_HET_28);

    /* Set GPIO output state */
    gpioOutputState |= (1<<PIN_HET_12);
    gpioOutputState |= (1<<PIN_HET_13);
    gpioOutputState |= (1<<PIN_HET_14);
    gpioOutputState |= (0<<PIN_HET_24);
    gpioOutputState |= (0<<PIN_HET_26);
    gpioOutputState |= (1<<PIN_HET_28); //Latch pin is high to start with

    gioSetDirection(hetPORT1, gpioDirectionConfig);
    gioSetPort(hetPORT1, gpioOutputState);

    /* Configure ADC on Photo Diode board */
    AdcConfig();
}

void OpticsDriver::AdcConfig(void)
{
    uint16_t *adcConfigPointer = NULL;
    uint16_t adcConfig = 0x0000;
    //uint16_t adcValue = 0x0000;

    adcConfigPointer = &adcConfig;

    adcConfig |= OVERWRITE_CFG << CFG_SHIFT;
    adcConfig |= UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT;
    adcConfig |= PDINPUTA1 << IN_CH_SEL_SHIFT;
    adcConfig |= FULL_BW << FULL_BW_SEL_SHIFT;
    adcConfig |= EXT_REF << REF_SEL_SHIFT;
    adcConfig |= DISABLE_SEQ << SEQ_EN_SHIFT;
    adcConfig |= READ_BACK_DISABLE << READ_BACK_SHIFT;
    adcConfig <<= 2;

    /* Send 2 dummy conversion to update config register on Photo Diode ADC */
    /* First dummy conversion */
    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, 14, 0); //Start sending command
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, 14, 1);
    for(int i=0; i<1000; i++); //Wait for sometime for conv/acq to complete

    /* Wait for conversion to complete ~ 3.2 uS */
    adcConfig = 0;
    /* Second dummy conversion */
    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, 14, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, 14, 1);
    for(int i=0; i<1000; i++);

    /* Set Configuration Value */
    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, 14, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, 14, 1);
    for(int i=0; i<1000; i++);
}

/**
 * Name: GetAdc()
 * Parameters: uint32_t nChanIdx: ADC channel to read
 * Returns: uint16_t nAdcVal: ADC value
 * Description: Obtain value from ADC
 */
uint16_t OpticsDriver::GetAdc(uint32_t nChanIdx)
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
    adcConfig |= EXT_REF << REF_SEL_SHIFT;
    adcConfig |= DISABLE_SEQ << SEQ_EN_SHIFT;
    adcConfig |= READ_BACK_DISABLE << READ_BACK_SHIFT;
    adcConfig <<= 2;

    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, 14, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, 14, 1);
    mibspiGetData(mibspiREG3, kpdAdcGroup, data); //Get ADC Value
    for(int i=0; i<1000; i++);

    adcConfig = 0;

    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, 14, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, 14, 1);
    mibspiGetData(mibspiREG3, kpdAdcGroup, data); //Get ADC Value
    for(int i=0; i<1000; i++);

    mibspiSetData(mibspiREG3, kpdAdcGroup, adcConfigPointer); //Set Config command
    gioSetBit(hetPORT1, 14, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, 14, 1);
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
    gioSetBit(hetPORT1, LATCH_PIN, 0);
    gioSetBit(hetPORT1, DATA_PIN, 0);

    switch (state)
    {
        case RESET_STATE:
            serialDataIn = 0x0000;
            break;
        case HOLD_STATE:
            serialDataIn = 0xFFFF;
            break;
        case INTEGRATE_STATE:
            //serialDataIn = 0x0000 | (uint8_t) npdChanIdx;
            serialDataIn = 0x00FF;
            break;
        default:
            break;
    }

    /* shift data into shift register */
    for (int i = 15; i>=0; i--)
    {
        gioSetBit(hetPORT1, CLK_PIN, 0);
        /* Checks if serial data is 1 or 0 shifts accordingly */
        if (serialDataIn & (1<<i))
        {
            pinState = 1;
        }
        else
        {
            pinState = 0;
        }
        gioSetBit(hetPORT1, DATA_PIN, pinState);
        gioSetBit(hetPORT1, CLK_PIN, 1);
        gioSetBit(hetPORT1, DATA_PIN, 0);
    }
    gioSetBit(hetPORT1, CLK_PIN, 0);
    //gioSetBit(hetPORT1, LATCH_PIN, 1);
}


void OpticsDriver::TimerTest(float period)
{
    //uint32_t tickTimeEnd = 0;
//    hetSIGNAL_t signal;
//
//    signal.duty = 50;
//    signal.period = period;

    gioSetBit(hetPORT1, 13, 0);
    for(int i=0; i<period; i++);
    gioSetBit(hetPORT1, 13, 1);
    //vTaskDelay(delay); // 1 tick is 562 uS
    //for(int i=0; i<266; i++); // @ 133 counts: delay of 10.073 uS

//    rtiStopCounter(rtiCOUNTER_BLOCK1);
//   while (!rtiResetCounter(rtiCOUNTER_BLOCK1));
//    rtiSetPeriod(rtiCOMPARE2, period);
//    rtiStartCounter(rtiCOUNTER_BLOCK1);
//    while (rtiGetCurrentTick(rtiCOMPARE2) < period);
//    rtiStopCounter(rtiCOUNTER_BLOCK1);
    //gioSetBit(hetPORT1, 13, 0);

    //pwmSetSignal(hetRAM1, pwm0, signal);

    //pwmEnableNotification(hetREG1, pwm0, pwmEND_OF_PERIOD);

    //pwmStart(hetRAM2, pwm0);
    //tickTimeEnd = rtiGetCurrentTick(rtiCOMPARE0) + period;
    //while (rtiGetCurrentTick(rtiCOMPARE0) < tickTimeEnd);
    //gioSetBit(hetPORT1, 13, 1);
}
