#include "libMutex.h"
#include "libPhotodiode.h"

SemaphoreHandle_t LibPhotodiode::s_mutex;
bool LibPhotodiode::s_isInitialized;

LibPhotodiode::LibPhotodiode() :
    m_integrationTimeInUs(10000),
    m_photodiodeThermistor(LibThermistor::NTCG163JF103FT1, LibThermistor::CELSIUS),
    m_led(SELECT_LED_BLUE1),
    m_photodiode(SELECT_PHOTODIODE_D11_T1)
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
    struct OpticsDriver::Data data;
    m_opticsDriver.GetPhotoDiodeValue(nledChanIdx, npdChanIdx, nDuration_us,
                                                          nLedIntensity, &data);
    m_photodiodeResultRaw = data.m_photodiodeResultRaw;
    float rt = convertRawDataToResistance(data.m_photodiodeTemperatureRaw);
    m_photodiodeTemperatureDuringIntegration = m_photodiodeThermistor.getTemperature(rt);
    float photoDiodeReading = m_photodiodeResultRaw * (m_opticsDriver.GetPhotodiodeVref() / 65535);
    return photoDiodeReading;
}

uint32 LibPhotodiode::readPhotodiodeRaw()
{
    return m_photodiodeResultRaw;
}

uint32 LibPhotodiode::readLedBoardVersion()
{
    LibMutex libMutex(s_mutex);
    struct OpticsDriver::BoardVersion boardVersion;
    m_opticsDriver.GetBoardVersion(boardVersion);
    return boardVersion.m_ledBoardVersion;
}

uint32 LibPhotodiode::readPhotodiodeBoardVersion()
{
    LibMutex libMutex(s_mutex);
    struct OpticsDriver::BoardVersion boardVersion;
    m_opticsDriver.GetBoardVersion(boardVersion);
    return boardVersion.m_photodiodeBoardVersion;
}

int LibPhotodiode::setLedBoardVersion(uint32 version)
{
    LibMutex libMutex(s_mutex);
    struct OpticsDriver::BoardVersion boardVersion;
    m_opticsDriver.GetBoardVersion(boardVersion);
    switch (version) {
    default:
        return ERROR_LED_BOARD_VERSION_INVALID;
    case OpticsDriver::LED_BOARD_V1:
    case OpticsDriver::LED_BOARD_V2:
        break;
    }
    boardVersion.m_ledBoardVersion = version;
    m_opticsDriver.SetBoardVersion(boardVersion);
    return OKAY;
}

int LibPhotodiode::setPhotodiodeBoardVersion(uint32 version)
{
    LibMutex libMutex(s_mutex);
    struct OpticsDriver::BoardVersion boardVersion;
    m_opticsDriver.GetBoardVersion(boardVersion);
    switch (version) {
    default:
        return ERROR_PHOTODIODE_BOARD_VERSION_INVALID;
    case OpticsDriver::PHOTODIODE_BOARD_V1:
    case OpticsDriver::PHOTODIODE_BOARD_V2:
        break;
    }
    boardVersion.m_photodiodeBoardVersion = version;
    m_opticsDriver.SetBoardVersion(boardVersion);
    return OKAY;
}

float LibPhotodiode::readLedTemperature()
{
    return 25;
}

float LibPhotodiode::readPhotodiodeTemperature()
{
    LibMutex libMutex(s_mutex);
    uint32_t npdChanIdx = m_pdMap[m_photodiode];
    struct OpticsDriver::Data data;
    m_opticsDriver.GetPhotoDiodeTemperatureRaw(npdChanIdx, &data);
    float rt = convertRawDataToResistance(data.m_photodiodeTemperatureRaw);
    float temperature = m_photodiodeThermistor.getTemperature(rt);
    return temperature;
}

float LibPhotodiode::readLedMonitorPhotodiodeDuringIntegration()
{
    return 25;
}

float LibPhotodiode::readLedTemperatureDuringIntegration()
{
    return 25;
}

float LibPhotodiode::readPhotodiodeTemperatureDuringIntegration()
{
    return m_photodiodeTemperatureDuringIntegration;
}

// TEMP_AINx = Vref * Rt/(10,700 + Rt)
// Rt = 10,700 / (Vref / TEMP_AINx - 1)
float LibPhotodiode::convertRawDataToResistance(uint16_t data)
{
    float voltage = data  * (m_opticsDriver.GetPhotodiodeVref() / 65535);
    if (voltage == 0 || voltage == m_opticsDriver.GetPhotodiodeVref()) {
        return 10000;
    }
    float rt = 10700 / (m_opticsDriver.GetPhotodiodeVref() / voltage - 1);
    return rt;
}

void LibPhotodiode::ledBoardEnable()
{
    m_opticsDriver.LedBoardEnable();
}

void LibPhotodiode::ledBoardDisable()
{
    m_opticsDriver.LedBoardDisable();
}

void LibPhotodiode::pdBoardEnable()
{
    m_opticsDriver.PhotodiodeBoardEnable();
}

void LibPhotodiode::pdBoardDisable()
{
    m_opticsDriver.PhotodiodeBoardDisable();
}

uint32 LibPhotodiode::getLedBoardEnabledStatus()
{
    return m_opticsDriver.IsLedBoardEnabled() ? LED_BOARD_ENABLED
                                              : LED_BOARD_DISABLED;
}

uint32 LibPhotodiode::getPhotodiodeBoardEnabledStatus()
{
    return m_opticsDriver.IsPhotodiodeBoardEnabled() ? PD_BOARD_ENABLED
                                                     : PD_BOARD_DISABLED;
}
