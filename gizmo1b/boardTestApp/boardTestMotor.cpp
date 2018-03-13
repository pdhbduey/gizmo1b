#include "boardTestMotor.h"

BoardTestMotor::BoardTestMotor()
{
}

BoardTestMotor::~BoardTestMotor()
{
}

int BoardTestMotor::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case MOTOR_STATUS:
        value = m_libMotor.getMotorStatus() << MOTOR_STATUS_SHIFT;
        break;
    case MOTOR_CONTROL:
        value = 0;
        value |= (m_direction ? DIRECTION_POSITIVE : DIRECTION_NEGATIVE)
              |   m_microSteps << MICRO_STEPS_SHIFT;
        break;
    case MOTOR_RELATIVE_POSITION:
        value = m_relPosition;
        break;
    case MOTOR_ABSOLUTE_POSITION:
        value = m_absPosition;
        break;
    case MOTOR_SPEED:
        value = *reinterpret_cast<uint32*>(&m_speed); // NOTE: PC side may be LE
        break;
    case MOTOR_ACCELERATION:
        value = *reinterpret_cast<uint32*>(&m_acceleration);
        break;
    case MOTOR_DECELERATION:
        value = *reinterpret_cast<uint32*>(&m_deceleration);
        break;
    case MOTOR_HOLD_CURRENT:
        value = *reinterpret_cast<uint32*>(&m_holdCurrent);
        break;
    }
    return OKAY;
}

int BoardTestMotor::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case MOTOR_STATUS:
        return ERROR_RO;
    case MOTOR_CONTROL:
        if (value & INITIALIZE) {
            m_libMotor.initialize();
        }
        else if (value & MOVE_RELATIVE) {
            m_libMotor.moveRelative(m_direction, m_relPosition);
        }
        else if (value & MOVE_ABSOLUTE) {
            m_libMotor.moveAbs(m_absPosition);
        }
        else {
            if (value & MICRO_STEPS_SET) {
                m_microSteps = (value & MICRO_STEPS) >> MICRO_STEPS_SHIFT;
                m_libMotor.setMicroSteps(m_microSteps);
            }
            if (value & DIRECTION_POSITIVE) {
                m_direction = LibMotor::POSITIVE;
            }
            if (value & DIRECTION_NEGATIVE) {
                m_direction = LibMotor::NEGATIVE;
            }
        }
        break;
    case MOTOR_RELATIVE_POSITION:
        m_relPosition = value;
        break;
    case MOTOR_ABSOLUTE_POSITION:
        m_absPosition = value;
        break;
    case MOTOR_SPEED:
        m_speed = *reinterpret_cast<float*>(&value);
        break;
    case MOTOR_ACCELERATION:
        m_acceleration = *reinterpret_cast<float*>(&value);
        break;
    case MOTOR_DECELERATION:
        m_deceleration = *reinterpret_cast<float*>(&value);
        break;
    case MOTOR_HOLD_CURRENT:
        m_holdCurrent = *reinterpret_cast<float*>(&value);
        break;
    }
    return OKAY;
}
