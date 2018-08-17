/*
 * OpticsDriver.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: z003xk2p
 *      https://github.com/GilRoss/AmpDetectFW
 */

#include <OpticsDriver.h>
#include <het.h>
#include <libDelay.h>

#define delay_mS 10000
#define delay_uS 10

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
}

/**
 * Name: SetLedState()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriver::SetLedState(uint32_t nChanIdx, bool bStateOn)
{
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
 * Name: SetLedsOff()
 * Parameters:
 * Returns:
 * Description: Turns off LED by setting intensity to 0.
 */
void OpticsDriver::SetLedsOff()
{
    for (int nChanIdx=0; nChanIdx < 6; nChanIdx++)
    {
        SetLedIntensity(nChanIdx, 0);
    }
}

/**
 * Name: GetPhotoDiodeValue()
 * Parameters:
 * Returns:
 * Description:
 */
uint32_t OpticsDriver::GetPhotoDiodeValue(uint32_t nledChanIdx, uint32_t npdChanIdx, uint32_t nDuration_us, uint32_t nLedIntensity)
{
    uint16_t adcValue = 0x0000;
    uint32_t adcChannel = 0;

    switch(npdChanIdx)
    {
    case 0:
        npdChanIdx = 0x0001 << PDINPUTA1;
        adcChannel = 0;
        break;
    case 1:
        npdChanIdx = 0x0001 << PDINPUTB1;
        adcChannel = 3;
        break;
    case 2:
        npdChanIdx = 0x0001 << PDINPUTA2;
        adcChannel = 1;
        break;
    case 3:
        npdChanIdx = 0x0001 << PDINPUTB2;
        adcChannel = 4;
        break;
    case 4:
        npdChanIdx = 0x0001 << PDINPUTA3;
        adcChannel = 2;
        break;
    case 5:
        npdChanIdx = 0x0001 << PDINPUTB3;
        adcChannel = 5;
        break;
    default:
        npdChanIdx = 0x0001 << PDINPUTA1;
        break;
    }

    /* Reset Integrator first */
    SetIntegratorState(RESET_STATE, npdChanIdx);
    gioSetBit(hetPORT1, _pdsr_latch_pin, 1); //Enable Reset State
    for(int i=0; i<delay_uS*500; i++); //Hold in reset state for 500 us

    /* Turn On LED */
    SetLedIntensity(nledChanIdx, nLedIntensity);

    SetIntegratorState(INTEGRATE_STATE, npdChanIdx);

    //gioSetBit(hetPORT1, PIN_HET_14, 1);

    taskENTER_CRITICAL();

    gioSetBit(hetPORT1, _pdsr_latch_pin, 1); //Enable Integration State (start integrating)

    //gioSetBit(hetPORT1, PIN_HET_14, 0);

    LibDelay::rtiReset();

    SetIntegratorState(HOLD_STATE, npdChanIdx); //Configure Hold state

    LibDelay::rtiMicrosecDelay(nDuration_us);

    gioSetBit(hetPORT1, _pdsr_latch_pin, 1);

    //gioSetBit(hetPORT1, PIN_HET_14, 1);

    taskEXIT_CRITICAL();

    /* Turn Off LED */
    SetLedsOff();

    for(int i=0; i<delay_uS*500; i++); //Hold for 500 us time before reading

    adcValue = GetAdc(adcChannel);

    SetIntegratorState(RESET_STATE, npdChanIdx);
    gioSetBit(hetPORT1, _pdsr_latch_pin, 1); //Enable Reset State
    for(int i=0; i<delay_uS*500; i++); //Hold in reset state for 500 us

    return (uint32_t)adcValue;
}


/**
 * Name: OpticsDriverInit()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriver::OpticsDriverInit(void)
{
}

void OpticsDriver::AdcConfig(void)
{
    uint16_t adcConfig = 0x0000;

    adcConfig |= OVERWRITE_CFG << CFG_SHIFT;
    adcConfig |= UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT;
    adcConfig |= PDINPUTA1 << IN_CH_SEL_SHIFT;
    adcConfig |= FULL_BW << FULL_BW_SEL_SHIFT;
    adcConfig |= INT_REF4_096_AND_TEMP_SENS << REF_SEL_SHIFT;
    adcConfig |= DISABLE_SEQ << SEQ_EN_SHIFT;
    adcConfig |= READ_BACK_EN << READ_BACK_SHIFT;
    adcConfig <<= 2;

    uint16_t cfg[2] = { adcConfig, 0 };

    gioSetBit(_somisw_gioport, _somisw_pin, _pd_somisw);

    /* Send 2 dummy conversion to update config register on Photo Diode ADC */
    /* First dummy conversion */
    mibspiSetData(mibspiREG3, kpdAdcGroup, cfg); //Set Config command
    gioSetBit(hetPORT1, _pd_cs_pin, 0); //Start sending command
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, _pd_cs_pin, 1);
    for(int i=0; i<1000; i++); //Wait for sometime for conv/acq to complete

    /* Wait for conversion to complete ~ 3.2 uS */
    cfg[0] = 0;
    /* Second dummy conversion */
    mibspiSetData(mibspiREG3, kpdAdcGroup, cfg); //Set Config command
    gioSetBit(hetPORT1, _pd_cs_pin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, _pd_cs_pin, 1);
    for(int i=0; i<1000; i++);

    /* Set Configuration Value */
    mibspiSetData(mibspiREG3, kpdAdcGroup, cfg); //Set Config command
    gioSetBit(hetPORT1, _pd_cs_pin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, _pd_cs_pin, 1);
    for(int i=0; i<1000; i++);

    //gioSetBit(_somisw_gioport, _pd_cs_pin, 0);
}

/**
 * Name: GetAdc()
 * Parameters: uint32_t nChanIdx: ADC channel to read
 * Returns: uint16_t nAdcVal: ADC value
 * Description: Obtain value from ADC
 */
uint16_t OpticsDriver::GetAdc(uint32_t nChanIdx)
{
    uint16_t nAdcVal[2] = { 0x0000, 0x0000 };
    uint16_t adcConfig = 0;

    /* Get Adc Value */
    adcConfig |= OVERWRITE_CFG << CFG_SHIFT;
    adcConfig |= UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT;
    adcConfig |= (uint16_t) nChanIdx << IN_CH_SEL_SHIFT;
    adcConfig |= FULL_BW << FULL_BW_SEL_SHIFT;
    adcConfig |= INT_REF4_096_AND_TEMP_SENS << REF_SEL_SHIFT;
    adcConfig |= DISABLE_SEQ << SEQ_EN_SHIFT;
    adcConfig |= READ_BACK_EN << READ_BACK_SHIFT;
    adcConfig <<= 2;

    uint16_t cfg[2] = { adcConfig, 0 };

    gioSetBit(_somisw_gioport, _somisw_pin, _pd_somisw);

    mibspiSetData(mibspiREG3, kpdAdcGroup, cfg); //Set Config command
    gioSetBit(hetPORT1, _pd_cs_pin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, _pd_cs_pin, 1);
    mibspiGetData(mibspiREG3, kpdAdcGroup, nAdcVal); //Get ADC Value
    for(int i=0; i<1000; i++);

    cfg[0] = 0;

    mibspiSetData(mibspiREG3, kpdAdcGroup, cfg); //Set Config command
    gioSetBit(hetPORT1, _pd_cs_pin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, _pd_cs_pin, 1);
    mibspiGetData(mibspiREG3, kpdAdcGroup, nAdcVal); //Get ADC Value
    for(int i=0; i<1000; i++);

    mibspiSetData(mibspiREG3, kpdAdcGroup, cfg); //Set Config command
    gioSetBit(hetPORT1, _pd_cs_pin, 0); //Start dummy conversion
    mibspiTransfer(mibspiREG3, kpdAdcGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kpdAdcGroup)));
    gioSetBit(hetPORT1, _pd_cs_pin, 1);
    mibspiGetData(mibspiREG3, kpdAdcGroup, nAdcVal); //Get ADC Value
    for(int i=0; i<1000; i++);

    return nAdcVal[0];
}

/**
 * Name: SetIntegratorState()
 * Parameters:
 * Returns:
 * Description: Utilizes shift register to set integrator state
 */

//Reset Mode
//a.      Hold Switch goes Low
//b.      Reset Switch goes Low
//c.      Wait 500us
//
//(2)   Integrate Mode
//a.      Hold Switch stays Low
//b.      Reset Switch goes High
//c.      Wait Integration Time
//
//(3)   Read ADC
//a.      Hold Switch goes High
//b.      Reset Switch goes High
//c.      Wait 500us
//d.      Read ADC
void OpticsDriver::SetIntegratorState(pdIntegratorState state, uint32_t npdChanIdx)
{
    uint8_t pinState = 0;
    uint16_t serialDataIn = 0;
    /* Initialize GPIOs for Shift Register: Pin */
    /* Pull-down Latch pin, Clk pin and Data pin */
    gioSetBit(hetPORT1, _pdsr_latch_pin, 0);
    gioSetBit(hetPORT1, _pdsr_data_pin, 0);

    switch (state)
    {
        case RESET_STATE:
            serialDataIn = 0x0000; // MSB=Hold(L), LSB=Reset(L)
            break;
        case HOLD_STATE:
            serialDataIn = 0xFFFF; // MSB=Hold(H), LSB=Reset(H)
            break;
        case INTEGRATE_STATE:
            serialDataIn = 0x00FF; // MSB=Hold(L), LSB=Reset(H)
            break;
        default:
            break;
    }

    /* shift data into shift register */
    for (int i = 15; i>=0; i--)
    {
        gioSetBit(hetPORT1, _pdsr_clk_pin, 0);
        /* Checks if serial data is 1 or 0 shifts accordingly */
        if (serialDataIn & (1<<i))
        {
            pinState = 1;
        }
        else
        {
            pinState = 0;
        }
        gioSetBit(hetPORT1, _pdsr_data_pin, pinState);
        gioSetBit(hetPORT1, _pdsr_clk_pin, 1);
        gioSetBit(hetPORT1, _pdsr_data_pin, 0);
    }
    gioSetBit(hetPORT1, _pdsr_clk_pin, 0);
}
