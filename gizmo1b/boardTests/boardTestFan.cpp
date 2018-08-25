#include <boardTests/boardTestFan.h>

BoardTestFan::BoardTestFan()
{
}

BoardTestFan::~BoardTestFan()
{
}

int BoardTestFan::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case FAN_PWM1_PERIOD_IN_US:
        {
            float v = m_libFan.getPwm1PeriodInUs();
            value = *reinterpret_cast<uint32*>(&v);
        }
        break;
    case FAN_PWM1_DUTY_CYCLE:
        value = m_libFan.getPwm1DutyCycle();
        break;
    case FAN_PWM2_PERIOD_IN_US:
        {
            float v = m_libFan.getPwm2PeriodInUs();
            value = *reinterpret_cast<uint32*>(&v);
        }
        break;
    case FAN_PWM2_DUTY_CYCLE:
        value = m_libFan.getPwm2DutyCycle();
        break;
    case FAN_STATUS:
        value = m_status;
        break;
    case FAN_SENSOR1_RPM:
        {
            float v = m_libFan.getSensor1Rpm();
            value = *reinterpret_cast<uint32*>(&v);
        }
        break;
    case FAN_SENSOR2_RPM:
        {
            float v = m_libFan.getSensor2Rpm();
            value = *reinterpret_cast<uint32*>(&v);
        }
        break;
    }
    return OKAY;
}

int BoardTestFan::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case FAN_STATUS:
    case FAN_SENSOR1_RPM:
    case FAN_SENSOR2_RPM:
        return ERROR_RO;
    case FAN_PWM1_PERIOD_IN_US:
        {
            float periodInUs = *reinterpret_cast<float*>(&value);
            m_status = m_libFan.setPwm1PeriodInUs(periodInUs);
        }
        break;
    case FAN_PWM1_DUTY_CYCLE:
        m_status = m_libFan.setPwm1DutyCycle(value);
        break;
    case FAN_PWM2_PERIOD_IN_US:
        {
            float periodInUs = *reinterpret_cast<float*>(&value);
            m_status = m_libFan.setPwm2PeriodInUs(periodInUs);
        }
        break;
    case FAN_PWM2_DUTY_CYCLE:
        m_status = m_libFan.setPwm2DutyCycle(value);
        break;
    }
    return OKAY;
}
