#include "libMutex.h"
#include "libPhotodiode.h"

SemaphoreHandle_t LibPhotodiode::s_mutex;
bool LibPhotodiode::s_isInitialized;

LibPhotodiode::LibPhotodiode() :
    m_integrationTimeInUs(10000),
    m_temperatureUnits(CELSIUS),
    m_ledBoardVersion(LED_BOARD_V1),
    m_photodiodeBoardVersion(PHOTODIODE_BOARD_V1)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
    m_ledMap[SELECT_LED_BLUE1] = OpticsDriver::kBlue1;
    m_ledMap[SELECT_LED_GREEN] = OpticsDriver::kGreen;
    m_ledMap[SELECT_LED_RED1]  = OpticsDriver::kRed1;
    m_ledMap[SELECT_LED_BROWN] = OpticsDriver::kBrown;
    m_ledMap[SELECT_LED_RED2]  = OpticsDriver::kRed2;
    m_ledMap[SELECT_LED_BLUE2] = OpticsDriver::kBlue2;
    m_pdMap[SELECT_PHOTODIODE_D11_T1] = 1;
    m_pdMap[SELECT_PHOTODIODE_D10_T1] = 2;
    m_pdMap[SELECT_PHOTODIODE_D11_T2] = 3;
    m_pdMap[SELECT_PHOTODIODE_D10_T2] = 4;
    m_pdMap[SELECT_PHOTODIODE_D11_T3] = 5;
    m_pdMap[SELECT_PHOTODIODE_D10_T3] = 6;
}

LibPhotodiode::~LibPhotodiode()
{
}

int LibPhotodiode::setLed(int led)
{
    LibMutex libMutex(s_mutex);
    switch (led) {
    default:
        return ERROR_SELECT_LED_OUT_OF_RANGE;
    case SELECT_LED_BLUE1:
    case SELECT_LED_GREEN:
    case SELECT_LED_RED1:
    case SELECT_LED_BROWN:
    case SELECT_LED_RED2:
    case SELECT_LED_BLUE2:
        m_led = led;
        break;
    }
    return OKAY;
}

int LibPhotodiode::setPhotodiode(int photodiode)
{
    LibMutex libMutex(s_mutex);
    switch (photodiode) {
    default:
        return ERROR_SELECT_PHOTODIODE_OUT_OF_RANGE;
    case SELECT_PHOTODIODE_D11_T1:
    case SELECT_PHOTODIODE_D10_T1:
    case SELECT_PHOTODIODE_D11_T2:
    case SELECT_PHOTODIODE_D10_T2:
    case SELECT_PHOTODIODE_D11_T3:
    case SELECT_PHOTODIODE_D10_T3:
        m_photodiode = photodiode;
        break;
    }
    return OKAY;
}

int LibPhotodiode::getLed()
{
    return m_led;
}

int LibPhotodiode::getPhotodiode()
{
    return m_photodiode;
}

uint32 LibPhotodiode::getIntegrationTimeInUs()
{
    return m_integrationTimeInUs;
}

int LibPhotodiode::setIntegrationTimeInUs(uint32 integrationTimeInUs)
{
    LibMutex libMutex(s_mutex);
    if (integrationTimeInUs < 1000 || integrationTimeInUs > 1000000) {
        return ERROR_INTEGRATION_TIME_OUT_OF_RANGE;
    }
    m_integrationTimeInUs = integrationTimeInUs;
    return OKAY;
}

uint32 LibPhotodiode::getLedIntensity()
{
    return m_ledIntensity;
}

int LibPhotodiode::setLedIntensity(uint32 ledIntensity)
{
    LibMutex libMutex(s_mutex);
    if (ledIntensity > 40000) {
        return ERROR_LED_INTENSITY_OUT_OF_RANGE;
    }
    m_ledIntensity = ledIntensity;
    return OKAY;
}

float LibPhotodiode::readPhotodiode()
{
    LibMutex libMutex(s_mutex);
    uint32_t nledChanIdx   = m_ledMap[m_led];
    uint32_t npdChanIdx    = m_pdMap[m_photodiode];
    uint32_t nDuration_us  = m_integrationTimeInUs;
    uint32_t nLedIntensity = m_ledIntensity;
    uint16_t data;
    m_opticsDriver.GetPhotoDiodeValue(nledChanIdx, npdChanIdx, nDuration_us,
                                                          nLedIntensity, &data);
    m_photodiodeResultRaw = data;
    float photoDiodeReading = data * (5.0 / 65535);
    return photoDiodeReading;
}

uint32 LibPhotodiode::readPhotodiodeRaw()
{
    return m_photodiodeResultRaw;
}

uint32 LibPhotodiode::readLedBoardVersion()
{
    return m_ledBoardVersion;
}

uint32 LibPhotodiode::readPhotodiodeBoardVersion()
{
    return m_photodiodeBoardVersion;
}

int LibPhotodiode::setLedBoardVersion(uint32 version)
{
    switch (version) {
    default:
        return ERROR_LED_BOARD_VERSION_INVALID;
    case LED_BOARD_V1:
    case LED_BOARD_V2:
        m_ledBoardVersion = version;
        break;
    }
    return OKAY;
}

int LibPhotodiode::setPhotodiodeBoardVersion(uint32 version)
{
    switch (version) {
    default:
        return ERROR_PHOTODIODE_BOARD_VERSION_INVALID;
    case PHOTODIODE_BOARD_V1:
    case PHOTODIODE_BOARD_V2:
        m_photodiodeBoardVersion = version;
        break;
    }
    return OKAY;
}

float LibPhotodiode::readLedTemperature()
{
    return 22.5;
}

float LibPhotodiode::readPhotodiodeTemperature()
{
    return -22.5;
}
