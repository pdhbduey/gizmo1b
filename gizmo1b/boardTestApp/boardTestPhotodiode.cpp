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
              | m_libPhotodiode.getPhotodiode();
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
        return ERROR_RO;
    case PHOTODIODE_CONTROL:
        {
            int led = value & LibPhotodiode::SELECT_LED_MASK;
            if (led) {
                m_status = m_libPhotodiode.setLed(led);
            }
            if (m_status != LibPhotodiode::OKAY) {
                break;
            }
            int photodiode = value & LibPhotodiode::SELECT_PHOTODIODE_MASK;
            if (value & photodiode) {
                m_status = m_libPhotodiode.setPhotodiode(photodiode);
            }
        }
        break;
    case PHOTODIODE_INTEGRATION_TIME:
        m_status = m_libPhotodiode.setIntegrationTimeInUs(value);
        break;
    case PHOTODIODE_LED_INTENSITY:
        m_status = m_libPhotodiode.setLedIntensity(value);
        break;
    }
    return OKAY;
}