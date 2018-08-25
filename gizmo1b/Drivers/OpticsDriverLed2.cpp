#include <LibDelay.h>
#include <OpticsDriverLed2.h>

OpticsDriverLed2::OpticsDriverLed2(uint32_t nSiteIdx)
{
    /* Initialize LED and PD Board Driver */
    OpticsDriverInit();
}

/**
 * Name: OpticsDriverInit()
 * Parameters:
 * Returns:
 * Description:
 */

void OpticsDriverLed2::OpticsDriverInit(void)
{
    _somisw_pin     = PIN_CS0;
    _somisw_gioport = mibspiPORT3;

    _adc_group  = 1;
    _adc_cs_pin = PIN_HET_12;
    _adc_somisw = SPI_A;

    _dac_group  = 2;
    _dac_cs_pin = PIN_HET_14;
    _dac_somisw = SPI_B;

    gioSetBit(hetPORT1, _adc_cs_pin, 1);

    /* Configure ADC on LED board */
    AdcConfig();

    // Configure DAC
    uint16_t nBitPattern[3];
    uint32 ctrl;
    ctrl           = DAC_AD5683R_WRITE_CTRL_REGISTER << 20
                   | DAC_AD5683R_RESET;
    nBitPattern[0] = ctrl >> 16;
    nBitPattern[1] = ctrl >> 8;
    nBitPattern[2] = ctrl;
    gioSetBit(_somisw_gioport, _somisw_pin, _dac_somisw);
    gioSetBit(hetPORT1, _dac_cs_pin, 0);
    mibspiSetData(mibspiREG3, _dac_group, nBitPattern);
    mibspiTransfer(mibspiREG3, _dac_group);
    while(!(mibspiIsTransferComplete(mibspiREG3, _dac_group)));
    gioSetBit(hetPORT1, _dac_cs_pin, 1);
    ctrl           = DAC_AD5683R_WRITE_CTRL_REGISTER << 20
                   | DAC_AD5683R_NORMAL_MODE
                   | DAC_AD5683R_ENABLE_REF
                   | DAC_AD5683R_OUT_2xVREF
                   | DAC_AD5683R_STANDALONE;
    nBitPattern[0] = ctrl >> 16;
    nBitPattern[1] = ctrl >> 8;
    nBitPattern[2] = ctrl;
    gioSetBit(_somisw_gioport, _somisw_pin, _dac_somisw);
    gioSetBit(hetPORT1, _dac_cs_pin, 0);
    mibspiSetData(mibspiREG3, _dac_group, nBitPattern);
    mibspiTransfer(mibspiREG3, _dac_group);
    while(!(mibspiIsTransferComplete(mibspiREG3, _dac_group)));
    gioSetBit(hetPORT1, _dac_cs_pin, 1);

    SetLedIntensity(0, 0);
}

/**
 * Name: SetLedIntensity()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriverLed2::SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity)
{
    uint16_t nBitPattern[3];
    uint32 ctrl;
    ctrl           = DAC_AD5683R_WRITE_DAC_AND_INPUT_REGISTER << 20
                   | nLedIntensity << 4;
    nBitPattern[0] = ctrl >> 16;
    nBitPattern[1] = ctrl >> 8;
    nBitPattern[2] = ctrl;
    gioSetBit(_somisw_gioport, _somisw_pin, _dac_somisw);
    gioSetBit(hetPORT1, _dac_cs_pin, 0);
    mibspiSetData(mibspiREG3, _dac_group, nBitPattern);
    mibspiTransfer(mibspiREG3, _dac_group);
    while(!(mibspiIsTransferComplete(mibspiREG3, _dac_group)));
    gioSetBit(hetPORT1, _dac_cs_pin, 1);
}
