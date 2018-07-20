#include "boardTestThermistor.h"

BoardTestThermistor::BoardTestThermistor()
{
}

BoardTestThermistor::~BoardTestThermistor()
{
}

int BoardTestThermistor::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case THERMISTOR_STATUS:
        value = m_status;
        break;
    case THERMISTOR_RESULT_AIN_A:
        {
            float ain;
            m_status = m_libThermistor.readTemp(LibThermistor::AIN_A, ain);
            if (m_status == LibThermistor::OKAY) {
                value = *reinterpret_cast<uint32*>(&ain);
            }
        }
        break;
    case THERMISTOR_RESULT_AIN_B:
        {
            float ain;
            m_status = m_libThermistor.readTemp(LibThermistor::AIN_B, ain);
            if (m_status == LibThermistor::OKAY) {
                value = *reinterpret_cast<uint32*>(&ain);
            }
        }
        break;
    case THERMISTOR_RESULT_AIN_C:
        {
            float ain;
            m_status = m_libThermistor.readTemp(LibThermistor::AIN_C, ain);
            if (m_status == LibThermistor::OKAY) {
                value = *reinterpret_cast<uint32*>(&ain);
            }
        }
        break;
    case THERMISTOR_RESULT_AIN_D:
        {
            float ain;
            m_status = m_libThermistor.readTemp(LibThermistor::AIN_D, ain);
            if (m_status == LibThermistor::OKAY) {
                value = *reinterpret_cast<uint32*>(&ain);
            }
        }
        break;
    case THERMISTOR_TYPE:
        switch (m_libThermistor.getType()) {
        default:
        case LibThermistor::SC30F103AN:
            value = SC30F103AN;
            break;
        case LibThermistor::USP12837:
            value = USP12837;
            break;
        }
        break;
    }
    return OKAY;
}

int BoardTestThermistor::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case THERMISTOR_STATUS:
    case THERMISTOR_RESULT_AIN_A:
    case THERMISTOR_RESULT_AIN_B:
    case THERMISTOR_RESULT_AIN_C:
    case THERMISTOR_RESULT_AIN_D:
        return ERROR_RO;
    case THERMISTOR_TYPE:
        m_status = (value & SC30F103AN ? m_libThermistor.setType(LibThermistor::SC30F103AN)
                 : (value & USP12837   ? m_libThermistor.setType(LibThermistor::USP12837)
                 :                       LibThermistor::ERROR_INVALID_TYPE));
        break;
    }
    return OKAY;
}
