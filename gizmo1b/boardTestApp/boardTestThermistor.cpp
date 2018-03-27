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
    }
    return OKAY;
}
