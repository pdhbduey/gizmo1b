#include <OpticsDriverPd1.h>

OpticsDriverPd1::OpticsDriverPd1(uint32_t nSiteIdx)
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
void OpticsDriverPd1::SetLedIntensity(uint32_t nChanIdx, uint32_t nLedIntensity)
{
    uint16_t nBitPattern[2] = {0xFF00, 0x0000};

    nBitPattern[0] = (uint16_t)(nBitPattern[0] | (((uint16_t)kwrInputupdateN << 4) | nChanIdx));
    /* Checks if Led Intensity exceeds 2.5 V ~ 40000 counts and caps it */
    if (nLedIntensity <= maxLedIntensity)
        nBitPattern[1] = (uint16_t)nLedIntensity;
    else
        nBitPattern[1] = (uint16_t)maxLedIntensity;
    gioSetBit(hetPORT1, LED_CS_PIN, 0);
    mibspiSetData(mibspiREG3, kledDacGroup, nBitPattern);
    mibspiTransfer(mibspiREG3, kledDacGroup);
    while(!(mibspiIsTransferComplete(mibspiREG3, kledDacGroup)));
    gioSetBit(hetPORT1, LED_CS_PIN, 1);
}

/**
 * Name: OpticsDriverInit()
 * Parameters:
 * Returns:
 * Description:
 */
void OpticsDriverPd1::OpticsDriverInit(void)
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

    /* Set GPIO pin direction - Disable LED functionality */
    //gpioDirectionConfig |= (1<<LED_LDAC_PIN);
    //gpioDirectionConfig |= (1<<LED_CS_PIN);
    gpioDirectionConfig |= (1<<PD_CS_PIN);
    gpioDirectionConfig |= (1<<PDSR_DATA_PIN);
    gpioDirectionConfig |= (1<<PDSR_CLK_PIN);
    gpioDirectionConfig |= (1<<PDSR_LATCH_PIN);

    /* Set GPIO output state */
    //gpioOutputState |= (1<<LED_LDAC_PIN);
    //gpioOutputState |= (1<<LED_CS_PIN);
    gpioOutputState |= (1<<PD_CS_PIN);
    gpioOutputState |= (0<<PDSR_DATA_PIN);
    gpioOutputState |= (0<<PDSR_CLK_PIN);
    gpioOutputState |= (1<<PDSR_LATCH_PIN); //Latch pin is high to start with

    gioSetDirection(hetPORT1, gpioDirectionConfig);
    gioSetPort(hetPORT1, gpioOutputState);

    /* Set GPIO pin direction to enable communication using mibSPI3 */
    pdAdcGpioConfig |= (1<<PIN_CS0);

    pdAdcGpioOutputState |= (1<<PIN_CS0);

    gioSetDirection(mibspiPORT3, pdAdcGpioConfig);
    gioSetPort(mibspiPORT3, pdAdcGpioOutputState);

    /* Configure ADC on Photo Diode board */
    AdcConfig();

    /* Disable PWM notification */
    pwmDisableNotification(hetREG1, pwm0, pwmEND_OF_BOTH);
}
