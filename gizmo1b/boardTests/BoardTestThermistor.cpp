#include <BoardTestThermistor.h>

BoardTestThermistor::BoardTestThermistor(LibThermistor* libThermistor, LibThermistor* libThermistorOnBoard) :
    m_libThermistor(*libThermistor),
    m_libThermistorOnBoard(libThermistorOnBoard)
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
    case THERMISTOR_RESULT_AIN_B:
    case THERMISTOR_RESULT_AIN_C:
    case THERMISTOR_RESULT_AIN_D:
        {
            float ain;
            int channel =  address - THERMISTOR_RESULT_AIN_A + LibThermistor::AIN_A;
            m_status = m_libThermistor.readTemp(channel, ain);
            if (m_status == LibThermistor::OKAY) {
                value = *reinterpret_cast<uint32*>(&ain);
            }
        }
        break;
    case THERMISTOR_TYPE:
        switch (m_libThermistor.getType()) {
        default:
        case LibThermistorCurves::SC30F103AN:
            value = SC30F103AN;
            break;
        case LibThermistorCurves::USP12837:
            value = USP12837;
            break;
        }
        break;
    case THERMISTOR_RESULT_INA1:
    case THERMISTOR_RESULT_INA2:
    case THERMISTOR_RESULT_INB1:
    case THERMISTOR_RESULT_INB2:
        {
            float ain   = 0;
            int channel = address - THERMISTOR_RESULT_INA1 + LibThermistor::INA1;
            m_status    = m_libThermistorOnBoard
                        ? m_libThermistorOnBoard->readTemp(channel, ain)
                        : LibThermistor::OKAY;
            if (m_status == LibThermistor::OKAY) {
                value = *reinterpret_cast<uint32*>(&ain);
            }
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
        m_status = (value & SC30F103AN ? m_libThermistor.setType(LibThermistorCurves::SC30F103AN)
                 : (value & USP12837   ? m_libThermistor.setType(LibThermistorCurves::USP12837)
                 :                       LibThermistorCurves::ERROR_INVALID_TYPE));
        break;
    }
    return OKAY;
}
