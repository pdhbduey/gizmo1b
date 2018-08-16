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
    /* Configure following GPIOs
     * LED GPIO: N2HET1[13] -> CS (Output/High); N2HET1[12] -> LDAC (Output/High)
     * PD ADC GPIO: N2HET1[14] -> CNV (Output/Low)
     * PD SR GPIO: N2HET1[24] -> DS (Serial Data In); N2HET1[26] -> SHCP (Shift Reg Clock Input)
     *             N2HET1[28] -> STCP (Storage Reg Clock Input)
     */
    uint32_t gpioDirectionConfig = 0x00000000;
    uint32_t gpioOutputState = 0x00000000;
    uint32_t pdAdcGpioConfig = 0x00000000;
    uint32_t pdAdcGpioOutputState = 0x00000000;

    _pd_cs_pin = PIN_HET_26;

    _pdsr_data_pin  = PIN_HET_12;
    _pdsr_clk_pin   = PIN_HET_13;
    _pdsr_latch_pin = PIN_HET_24;
    _pd_pwm         = pwm2;

    _somisw_pin     = PIN_CS0;
    _somisw_gioport = mibspiPORT3;
    _pd_somisw      = SPI_A;

    /* Set GPIO pin direction - Disable LED functionality */
    gpioDirectionConfig |= (1<<_pd_cs_pin);
    gpioDirectionConfig |= (1<<_pdsr_data_pin);
    gpioDirectionConfig |= (1<<_pdsr_clk_pin);
    gpioDirectionConfig |= (1<<_pdsr_latch_pin);

    /* Set GPIO output state */
    gpioOutputState |= (1<<_pd_cs_pin);
    gpioOutputState |= (0<<_pdsr_data_pin);
    gpioOutputState |= (0<<_pdsr_clk_pin);
    gpioOutputState |= (1<<_pdsr_latch_pin); //Latch pin is high to start with

    gioSetDirection(hetPORT1, gpioDirectionConfig);
    gioSetPort(hetPORT1, gpioOutputState);

    /* Set GPIO pin direction to enable communication using mibSPI3 */
    pdAdcGpioConfig |= (1<<_somisw_pin);

    pdAdcGpioOutputState |= (1<<_somisw_pin);

    gioSetDirection(mibspiPORT3, pdAdcGpioConfig);
    gioSetPort(mibspiPORT3, pdAdcGpioOutputState);

    /* Configure ADC on Photo Diode board */
    AdcConfig();

    /* Disable PWM notification */
    pwmDisableNotification(hetREG1, _pd_pwm, pwmEND_OF_BOTH);
}
