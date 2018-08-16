#include <OpticsDriverPd2.h>

OpticsDriverPd2::OpticsDriverPd2(uint32_t nSiteIdx)
{
    /* Initialize LED and PD Board Driver */
    OpticsDriverInit();
}

/**
 * Name: SetLedIntensity()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriverPd2::SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity)
{
}

/**
 * Name: OpticsDriverInit()
 * Parameters:
 * Returns:
 * Description:
 */

void OpticsDriverPd2::OpticsDriverInit(void)
{
    _somisw_pin     = PIN_CS0;
    _somisw_gioport = mibspiPORT3;

    _pd_cs_pin = PIN_HET_26;
    _pd_somisw = SPI_A;

    _pdsr_data_pin  = PIN_HET_12;
    _pdsr_clk_pin   = PIN_HET_13;
    _pdsr_latch_pin = PIN_HET_24;
    _pd_pwm         = pwm2;

    gioSetBit(hetPORT1, _pd_cs_pin,      1);
    gioSetBit(hetPORT1, _pdsr_data_pin,  0);
    gioSetBit(hetPORT1, _pdsr_clk_pin,   0);
    gioSetBit(hetPORT1, _pdsr_latch_pin, 1); //Latch pin is high to start with

    /* Configure ADC on Photo Diode board */
    AdcConfig();

    /* Disable PWM notification */
    pwmDisableNotification(hetREG1, _pd_pwm, pwmEND_OF_BOTH);
}
