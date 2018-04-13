#include <map>
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
    case MOTOR_COMMUNICATION_STATUS:
        value = m_status;
        break;
    case MOTOR_CONTROL:
        {
            int microSteps;
            m_status = m_libMotor.getMicroSteps(microSteps);
            if (m_status == LibMotor::OKAY) {
                std::map<int, int> step;
                step[LibMotor::STEP_FULL]          = BoardTestMotor::STEP_FULL;
                step[LibMotor::STEP_HALF]          = BoardTestMotor::STEP_HALF;
                step[LibMotor::STEP_MICRO_1_4TH]   = BoardTestMotor::STEP_MICRO_1_4TH;
                step[LibMotor::STEP_MICRO_1_8TH]   = BoardTestMotor::STEP_MICRO_1_8TH;
                step[LibMotor::STEP_MICRO_1_16TH]  = BoardTestMotor::STEP_MICRO_1_16TH;
                step[LibMotor::STEP_MICRO_1_32TH]  = BoardTestMotor::STEP_MICRO_1_32TH;
                step[LibMotor::STEP_MICRO_1_64TH]  = BoardTestMotor::STEP_MICRO_1_64TH;
                step[LibMotor::STEP_MICRO_1_128TH] = BoardTestMotor::STEP_MICRO_1_128TH;
                value = step[microSteps];
            }
        }
        break;
    case MOTOR_RELATIVE_POSITION:
        value = m_relPosition;
        break;
    case MOTOR_ABSOLUTE_POSITION:
        value = *reinterpret_cast<uint32*>(&m_absPosition);
        break;
    case MOTOR_SPEED:
        break;
    case MOTOR_ACCELERATION:
        break;
    case MOTOR_DECELERATION:
        break;
    case MOTOR_CURRENT_POSITION:
        {
            sint32 position;
            m_status = m_libMotor.getPosition(position);
            if (m_status == LibMotor::OKAY) {
                value = *reinterpret_cast<uint32*>(&position);
            }
        }
        break;
    case MOTOR_STATUS:
        m_status = m_libMotor.getStatus(value);
        break;
    case MOTOR_REGISTER_ADDRESS:
        value = m_registerAddress;
        break;
    case MOTOR_REGISTER_VALUE:
        value = m_registerValue;
        break;
    }
    return OKAY;
}

int BoardTestMotor::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case MOTOR_COMMUNICATION_STATUS:
    case MOTOR_STATUS:
    case MOTOR_CURRENT_POSITION:
        return ERROR_RO;
    case MOTOR_CONTROL:
        if (value & INITIALIZE) {
            m_status = m_libMotor.initialize();
        }
        else if (value & MOVE_MASK) {
            switch (value & MOVE_MASK) {
            default:
                break;
            case LIMP:
                m_status = m_libMotor.limp();
                break;
            case ENERGIZE:
                m_status = m_libMotor.energize();
                break;
            case MOVE_ABSOLUTE:
                m_status = value & DIRECTION_REVERSE
                         ? m_libMotor.moveToPosition(LibMotor::REVERSE, m_absPosition)
                         : value & DIRECTION_FORWARD
                         ? m_libMotor.moveToPosition(LibMotor::FORWARD, m_absPosition)
                         : LibMotor::ERROR_INVALID_DIRECTION;
                break;
            case MOVE_RELATIVE:
                m_status = value & DIRECTION_REVERSE
                         ? m_libMotor.moveRelative(LibMotor::REVERSE, m_relPosition)
                         : value & DIRECTION_FORWARD
                         ? m_libMotor.moveRelative(LibMotor::FORWARD, m_relPosition)
                         : LibMotor::ERROR_INVALID_DIRECTION;
                break;
            }
        }
        else if (value & STEP_MASK) {
            std::map<int, int> step;
            step[BoardTestMotor::STEP_FULL]          = LibMotor::STEP_FULL;
            step[BoardTestMotor::STEP_HALF]          = LibMotor::STEP_HALF;
            step[BoardTestMotor::STEP_MICRO_1_4TH]   = LibMotor::STEP_MICRO_1_4TH;
            step[BoardTestMotor::STEP_MICRO_1_8TH]   = LibMotor::STEP_MICRO_1_8TH;
            step[BoardTestMotor::STEP_MICRO_1_16TH]  = LibMotor::STEP_MICRO_1_16TH;
            step[BoardTestMotor::STEP_MICRO_1_32TH]  = LibMotor::STEP_MICRO_1_32TH;
            step[BoardTestMotor::STEP_MICRO_1_64TH]  = LibMotor::STEP_MICRO_1_64TH;
            step[BoardTestMotor::STEP_MICRO_1_128TH] = LibMotor::STEP_MICRO_1_128TH;
            value &= STEP_MASK;
            if (step.find(value) != step.end()) {
                m_status = m_libMotor.setMicroSteps(step[value]);
            }
            else {
                m_status = LibMotor::ERROR_INVALID_MICROSTEPS;
            }
        }
        else if (value & REGISTER_READ) {
            uint32 v;
            m_status = m_libMotor.readReg(m_registerAddress, v);
            if (m_status == LibMotor::OKAY) {
                m_registerValue = v;
            }
        }
        else if (value & REGISTER_WRITE) {
            m_status = m_libMotor.writeReg(m_registerAddress, m_registerValue);
        }
        else if (value & RESET) {
            m_libMotor.reset();
        }
        break;
    case MOTOR_RELATIVE_POSITION:
        m_relPosition = value;
        break;
    case MOTOR_ABSOLUTE_POSITION:
        m_absPosition = *reinterpret_cast<sint32*>(&value);
        break;
    case MOTOR_SPEED:
        break;
    case MOTOR_ACCELERATION:
        break;
    case MOTOR_DECELERATION:
        break;
    case MOTOR_REGISTER_ADDRESS:
        m_registerAddress = value;
        break;
    case MOTOR_REGISTER_VALUE:
        m_registerValue = value;
        break;
    }
    return OKAY;
}
