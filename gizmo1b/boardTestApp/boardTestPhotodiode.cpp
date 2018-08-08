#include "boardTestPhotodiode.h"

BoardTestPhotodiode::BoardTestPhotodiode()
{
}

BoardTestPhotodiode::~BoardTestPhotodiode()
{
}

int BoardTestPhotodiode::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case PHOTODIODE_CONTROL:
        value = m_libPhotodiode.getLed()
              | m_libPhotodiode.getPhotodiode()
              | m_libPhotodiode.getLedBoardEnabledStatus()
              | m_libPhotodiode.getPhotodiodeBoardEnabledStatus()
              | m_libPhotodiode.getLedState();
        break;
    case PHOTODIODE_STATUS:
        value  = m_status;
        break;
    case PHOTODIODE_INTEGRATION_TIME:
        value = m_libPhotodiode.getIntegrationTimeInUs();
        break;
    case PHOTODIODE_LED_INTENSITY:
        value = m_libPhotodiode.getLedIntensity();
        break;
    case PHOTODIODE_READING_IN_VOLTS:
        {
            float photoDiodeReading = m_libPhotodiode.readPhotodiode();
            value = *reinterpret_cast<uint32*>(&photoDiodeReading);
        }
        break;
    case PHOTODIODE_READING_RAW:
        value = m_libPhotodiode.readPhotodiodeRaw();
        break;
    case PHOTODIODE_LED_BOARD_VERSION:
        value = m_libPhotodiode.readLedBoardVersion();
        break;
    case PHOTODIODE_PD_BOARD_VERSION:
        value = m_libPhotodiode.readPhotodiodeBoardVersion();
        break;
    case PHOTODIODE_LED_TEMPERATURE:
        {
            float temperature = m_libPhotodiode.readLedTemperature();
            value = *reinterpret_cast<uint32*>(&temperature);
        }
        break;
    case PHOTODIODE_PD_TEMPERATURE:
        {
            float temperature = m_libPhotodiode.readPhotodiodeTemperature();
            value = *reinterpret_cast<uint32*>(&temperature);
        }
        break;
    case PHOTODIODE_LED_MONITOR_PD_READING_DURING_INTEGRATION_IN_VOLTS:
        {
            float volts = m_libPhotodiode.readLedMonitorPhotodiodeDuringIntegration();
            value = *reinterpret_cast<uint32*>(&volts);
        }
        break;
    case PHOTODIODE_LED_TEMPERATURE_DURING_INTEGRATION:
        {
            float temperature = m_libPhotodiode.readLedTemperatureDuringIntegration();
            value = *reinterpret_cast<uint32*>(&temperature);
        }
        break;
    case PHOTODIODE_PD_TEMPERATURE_DURING_INTEGRATION:
        {
            float temperature = m_libPhotodiode.readPhotodiodeTemperatureDuringIntegration();
            value = *reinterpret_cast<uint32*>(&temperature);
        }
        break;
    case PHOTODIODE_LED_MONITOR_PD_READING_IN_VOLTS:
        {
            float volts = m_libPhotodiode.readLedMonitorPhotodiode();
            value = *reinterpret_cast<uint32*>(&volts);
        }
        break;
    }
    return OKAY;
}

int BoardTestPhotodiode::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case PHOTODIODE_READING_IN_VOLTS:
    case PHOTODIODE_READING_RAW:
    case PHOTODIODE_STATUS:
    case PHOTODIODE_LED_TEMPERATURE:
    case PHOTODIODE_PD_TEMPERATURE:
    case PHOTODIODE_LED_MONITOR_PD_READING_DURING_INTEGRATION_IN_VOLTS:
    case PHOTODIODE_LED_TEMPERATURE_DURING_INTEGRATION:
    case PHOTODIODE_PD_TEMPERATURE_DURING_INTEGRATION:
    case PHOTODIODE_LED_MONITOR_PD_READING_IN_VOLTS:
        return ERROR_RO;
    case PHOTODIODE_CONTROL:
        {
            if (value & LibPhotodiode::SELECT_LED_MASK) {
                m_status = m_libPhotodiode.setLed(value & LibPhotodiode::SELECT_LED_MASK);
                if (m_status != LibPhotodiode::OKAY) {
                    break;
                }
            }
            if (value & LibPhotodiode::SELECT_PHOTODIODE_MASK) {
                m_status = m_libPhotodiode.setPhotodiode(value & LibPhotodiode::SELECT_PHOTODIODE_MASK);
                if (m_status != LibPhotodiode::OKAY) {
                    break;
                }
            }
            if (value & LibPhotodiode::LED_BOARD_ENABLED) {
                m_libPhotodiode.ledBoardEnable();
            }
            else if (value & LibPhotodiode::LED_BOARD_DISABLED) {
                m_libPhotodiode.ledBoardDisable();
            }
            if (value & LibPhotodiode::PD_BOARD_ENABLED) {
                m_libPhotodiode.pdBoardEnable();
            }
            else if (value & LibPhotodiode::PD_BOARD_DISABLED) {
                m_libPhotodiode.pdBoardDisable();
            }
            if (value & LibPhotodiode::LED_TURN_ON) {
                m_libPhotodiode.ledTurnOn();
            }
            else if (value & LibPhotodiode::LED_TURN_OFF) {
                m_libPhotodiode.ledTurnOff();
            }
        }
        break;
    case PHOTODIODE_INTEGRATION_TIME:
        m_status = m_libPhotodiode.setIntegrationTimeInUs(value);
        break;
    case PHOTODIODE_LED_INTENSITY:
        m_status = m_libPhotodiode.setLedIntensity(value);
        break;
    case PHOTODIODE_LED_BOARD_VERSION:
        m_status = m_libPhotodiode.setLedBoardVersion(value);
        break;
    case PHOTODIODE_PD_BOARD_VERSION:
        m_status = m_libPhotodiode.setPhotodiodeBoardVersion(value);
        break;
    }
    return OKAY;
}
