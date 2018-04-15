#include "libFan.h"
//#include "libWrapHet.h"

//23:N2HET1[1]:FAN_SENSOR1
//24:N2HET1[3]:FAN_SENSOR2
//25:N2HET1[0]:FAN_PWM1
//30:N2HET1[2]:FAN_PWM2

LibFan::LibFan()
{
}

LibFan::~LibFan()
{
}

int LibFan::setPwm1DutyCycle(uint32 dutyCycle)
{
    return m_libWrapHet1.setPwmDutyCycle(LibWrapHet::PWM_0, dutyCycle);
}

int LibFan::setPwm2DutyCycle(uint32 dutyCycle)
{
    return m_libWrapHet1.setPwmDutyCycle(LibWrapHet::PWM_1, dutyCycle);
}

uint32 LibFan::getPwm1DutyCycle()
{
    return m_libWrapHet1.getPwmDutyCycle(LibWrapHet::PWM_0);
}

uint32 LibFan::getPwm2DutyCycle()
{
    return m_libWrapHet1.getPwmDutyCycle(LibWrapHet::PWM_1);
}

int LibFan::setPwm1PeriodInUs(float periodInUs)
{
    return m_libWrapHet1.setPwmPeriodInUs(LibWrapHet::PWM_0, periodInUs);
}

int LibFan::setPwm2PeriodInUs(float periodInUs)
{
    return m_libWrapHet1.setPwmPeriodInUs(LibWrapHet::PWM_1, periodInUs);
}

float LibFan::getPwm1PeriodInUs()
{
    return m_libWrapHet1.getPwmPeriodInUs(LibWrapHet::PWM_0);
}

float LibFan::getPwm2PeriodInUs()
{
    return m_libWrapHet1.getPwmPeriodInUs(LibWrapHet::PWM_1);
}

float LibFan::getSensor1Rpm()
{
    float periodInUs = m_libWrapHet1.getCapPeriodInUs(LibWrapHet::CAP_0);
    return 30.0 / (periodInUs * 1e-6); // AUB0812VH-SP00
}

float LibFan::getSensor2Rpm()
{
    float periodInUs = m_libWrapHet1.getCapPeriodInUs(LibWrapHet::CAP_1);
    return 30.0 / (periodInUs * 1e-6); // AUB0812VH-SP00
}
