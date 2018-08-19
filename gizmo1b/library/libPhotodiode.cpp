#include <FreeRTOS.h>
#include <os_task.h>
#include <libMutex.h>
#include <libPhotodiode.h>

// NOTE: I found that there is an issue with
// FAN_PWM1 (pwm0) on HET1[0] creating noise
// breaking communication over MIBSPI3 with the PD2.0
// board. It may be an issue with any communication
// over MIBSPI3 so it's safer to disable both pwm0 and pwm1 before
// any test related to MISPI3 is performed. The workaround
// is to keep both pwm0 and pwm1 disabled until both PD and LED boards
// are disabled

SemaphoreHandle_t LibPhotodiode::s_mutex;
bool LibPhotodiode::s_isInitialized;

LibPhotodiode::LibPhotodiode() :
    m_integrationTimeInUs(10000),
    m_ledThermistorCurve(LibThermistorCurves::NTCG163JF103FT1, LibThermistorCurves::CELSIUS),
    m_ledState(LED_TURN_OFF),
    m_libPdBoard(0),
    m_libLedBoard(0),
    m_led(LibLedBoard::SELECT_LED_BLUE1),
    m_photodiode(LibPdBoard::SELECT_PHOTODIODE_1),
    m_ledBoardVersion(LibLedBoard::LED_BOARD_V1),
    m_pdBoardVersion(LibPdBoard::PHOTODIODE_BOARD_V1)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibPhotodiode::~LibPhotodiode()
{
}

int LibPhotodiode::setLed(int led)
{
    LibMutex libMutex(s_mutex);
    int result = m_libLedBoard ? m_libLedBoard->setLed(led) : OKAY;
    m_led = led;
    return result;
}

int LibPhotodiode::getLed()
{
    LibMutex libMutex(s_mutex);
    return m_libLedBoard ? m_libLedBoard->getLed() : m_led;
}

int LibPhotodiode::setPhotodiode(uint32 photodiode)
{
    LibMutex libMutex(s_mutex);
    switch (photodiode) {
    default:
        return LibPdBoard::ERROR_SELECT_PHOTODIODE_OUT_OF_RANGE;
    case LibPdBoard::SELECT_PHOTODIODE_1:
    case LibPdBoard::SELECT_PHOTODIODE_2:
    case LibPdBoard::SELECT_PHOTODIODE_3:
    case LibPdBoard::SELECT_PHOTODIODE_4:
    case LibPdBoard::SELECT_PHOTODIODE_5:
    case LibPdBoard::SELECT_PHOTODIODE_6:
        m_photodiode = photodiode;
        break;
    }
    if (m_libPdBoard) {
        m_libPdBoard->setPhotodiode(photodiode);
    }
    return OKAY;
}

uint32 LibPhotodiode::getPhotodiode()
{
    LibMutex libMutex(s_mutex);
    return m_libPdBoard ? m_libPdBoard->getPhotodiode() : m_photodiode;
}

uint32 LibPhotodiode::getIntegrationTimeInUs()
{
    LibMutex libMutex(s_mutex);
    return m_libPdBoard ? m_libPdBoard->getIntegrationTimeInUs()
                        : m_integrationTimeInUs;
}

int LibPhotodiode::setIntegrationTimeInUs(uint32 integrationTimeInUs)
{
    LibMutex libMutex(s_mutex);
    if (integrationTimeInUs < 1000 || integrationTimeInUs > 1000000) {
        return LibPdBoard::ERROR_INTEGRATION_TIME_OUT_OF_RANGE;
    }
    m_integrationTimeInUs = integrationTimeInUs;
    if (m_libPdBoard) {
        m_libPdBoard->setIntegrationTimeInUs(integrationTimeInUs);
    }
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
    m_ledTemperatureDuringIntegration            = 0;
    m_ledMontorPhotodiodeResultDuringIntegration = 0;
    float result = m_libPdBoard ? m_libPdBoard->readPhotodiodeResult() : 0;
    return result;
}

uint32 LibPhotodiode::readPhotodiodeRaw()
{
    LibMutex libMutex(s_mutex);
    float result = m_libPdBoard ? m_libPdBoard->readPhotodiodeResultRaw() : 0;
    return result;
}

uint32 LibPhotodiode::readLedBoardVersion()
{
    LibMutex libMutex(s_mutex);
    return m_libPdBoard ? m_libPdBoard->getVersion() : m_ledBoardVersion;
}

int LibPhotodiode::setLedBoardVersion(uint32 version)
{
    LibMutex libMutex(s_mutex);
    int result = OKAY;
    switch (version) {
    default:
        result = ERROR_LED_BOARD_VERSION_INVALID;
        break;
    case LibLedBoard::LED_BOARD_V1:
    case LibLedBoard::LED_BOARD_V2:
        if (m_ledBoardVersion != version) {
            if (m_isLedBoardEnabled) {
                if (m_libLedBoard) {
                    m_led = m_libLedBoard->getLed();
                    delete m_libLedBoard;
                }
                switch (version) {
                default:
                    m_libLedBoard = 0;
                    break;
                case LibLedBoard::LED_BOARD_V2:
                    pwmStop(hetRAM1, pwm0);
                    pwmStop(hetRAM1, pwm1);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    m_libLedBoard = new LibLedBoardVersion2;
                    break;
                }
                if (m_libLedBoard) {
                    m_libLedBoard->setLed(m_led);
                }
            }
            m_ledBoardVersion = version;
        }
        break;
    }
    if (!m_libLedBoard && !m_libPdBoard) {
        pwmStart(hetRAM1, pwm0);
        pwmStart(hetRAM1, pwm1);
    }
    return result;
}

uint32 LibPhotodiode::readPhotodiodeBoardVersion()
{
    LibMutex libMutex(s_mutex);
    return m_libPdBoard ? m_libPdBoard ->getVersion() : m_pdBoardVersion;
}

int LibPhotodiode::setPhotodiodeBoardVersion(uint32 version)
{
    LibMutex libMutex(s_mutex);
    int result = OKAY;
    switch (version) {
    default:
        result = ERROR_PHOTODIODE_BOARD_VERSION_INVALID;
        break;
    case LibPdBoard::PHOTODIODE_BOARD_V1:
    case LibPdBoard::PHOTODIODE_BOARD_V2:
        if (m_pdBoardVersion != version) {
            if (m_isPdBoardEnabled) {
                if (m_libPdBoard) {
                    m_photodiode = m_libPdBoard->getPhotodiode();
                    delete m_libPdBoard;
                }
                switch (version) {
                default:
                    m_libPdBoard = 0;
                    break;
                case LibPdBoard::PHOTODIODE_BOARD_V2:
                    pwmStop(hetRAM1, pwm0);
                    pwmStop(hetRAM1, pwm1);
                    vTaskDelay(pdMS_TO_TICKS(100));
                    m_libPdBoard = new LibPdBoardVersion2;
                    break;
                }
                if (m_libPdBoard) {
                    m_libPdBoard->setPhotodiode(m_photodiode);
                }
            }
            m_pdBoardVersion = version;
        }
        break;
    }
    if (!m_libLedBoard && !m_libPdBoard) {
        pwmStart(hetRAM1, pwm0);
        pwmStart(hetRAM1, pwm1);
    }
    return result;
}

float LibPhotodiode::readLedTemperature()
{
    LibMutex libMutex(s_mutex);
    float temperature = m_libLedBoard ? m_libLedBoard->readLedTemperature() : 0;
    return temperature;
}

float LibPhotodiode::readPhotodiodeTemperature()
{
    LibMutex libMutex(s_mutex);
    float temperature = m_libPdBoard ? m_libPdBoard->readPhotodiodeTemperature() : 0;
    return temperature;
}

float LibPhotodiode::readLedMonitorPhotodiodeDuringIntegration()
{
    return m_ledMontorPhotodiodeResultDuringIntegration;
}

float LibPhotodiode::readLedTemperatureDuringIntegration()
{
    return m_ledTemperatureDuringIntegration;
}

float LibPhotodiode::readPhotodiodeTemperatureDuringIntegration()
{
    LibMutex libMutex(s_mutex);
    float temperature = m_libPdBoard ? m_libPdBoard->readPhotodiodeTemperatureDuringIntegration() : 0;
    return temperature;
}

void LibPhotodiode::ledBoardEnable()
{
    LibMutex libMutex(s_mutex);
    if (!m_isLedBoardEnabled) {
        m_isLedBoardEnabled = true;
        if (!m_libLedBoard) {
            switch (m_ledBoardVersion) {
            case LibLedBoard::LED_BOARD_V2:
                pwmStop(hetRAM1, pwm0);
                pwmStop(hetRAM1, pwm1);
                vTaskDelay(pdMS_TO_TICKS(100));
                m_libLedBoard = new LibLedBoardVersion2;
                break;
            }
            if (m_libLedBoard) {
                m_libLedBoard->setLed(m_led);
            }
        }
    }
}

void LibPhotodiode::ledBoardDisable()
{
    LibMutex libMutex(s_mutex);
    if (m_isLedBoardEnabled) {
        m_isLedBoardEnabled = false;
        if (m_libLedBoard) {
            m_led = m_libLedBoard->getLed();
            delete m_libLedBoard;
            m_libLedBoard = 0;
        }
    }
    if (!m_libLedBoard && !m_libPdBoard) {
        pwmStart(hetRAM1, pwm0);
        pwmStart(hetRAM1, pwm1);
    }
}

void LibPhotodiode::pdBoardEnable()
{
    LibMutex libMutex(s_mutex);
    if (!m_isPdBoardEnabled) {
        m_isPdBoardEnabled = true;
        if (!m_libPdBoard) {
            switch (m_pdBoardVersion) {
            case LibPdBoard::PHOTODIODE_BOARD_V2:
                pwmStop(hetRAM1, pwm0);
                pwmStop(hetRAM1, pwm1);
                vTaskDelay(pdMS_TO_TICKS(100));
                m_libPdBoard = new LibPdBoardVersion2;
                break;
            }
            if (m_libPdBoard) {
                m_libPdBoard->setPhotodiode(m_photodiode);
            }
        }
    }
}

void LibPhotodiode::pdBoardDisable()
{
    LibMutex libMutex(s_mutex);
    if (m_isPdBoardEnabled) {
        m_isPdBoardEnabled = false;
        if (m_libPdBoard) {
            m_photodiode = m_libPdBoard->getPhotodiode();
            delete m_libPdBoard;
            m_libPdBoard = 0;
        }
    }
    if (!m_libLedBoard && !m_libPdBoard) {
        pwmStart(hetRAM1, pwm0);
        pwmStart(hetRAM1, pwm1);
    }
}

uint32 LibPhotodiode::getLedBoardEnabledStatus()
{
    return m_libLedBoard ? LED_BOARD_ENABLED : LED_BOARD_DISABLED;
}

uint32 LibPhotodiode::getPhotodiodeBoardEnabledStatus()
{
    return m_libPdBoard ? PD_BOARD_ENABLED : PD_BOARD_DISABLED;
}

float LibPhotodiode::readLedMonitorPhotodiode()
{
    LibMutex libMutex(s_mutex);
//    uint32_t nledChanIdx = m_ledMap[m_led];
//    struct OpticsDriver::Data data;
//    m_opticsDriver.GetLedDataRaw(nledChanIdx, &data);
//    float volts = data.m_ledMontorPhotodiodeResultRaw
//                * (m_opticsDriver.GetLedVref() / 65535);
//    return volts;
    return 0;
}

uint32 LibPhotodiode::getLedState()
{
    return m_ledState;
}

void LibPhotodiode::ledTurnOn()
{
    LibMutex libMutex(s_mutex);
    m_ledState           = LED_TURN_ON;
//    uint32_t nledChanIdx = m_ledMap[m_led];
//    m_opticsDriver.SetLedIntensity(nledChanIdx, m_ledIntensity);
}

void LibPhotodiode::ledTurnOff()
{
    LibMutex libMutex(s_mutex);
    m_ledState           = LED_TURN_OFF;
//    uint32_t nledChanIdx = m_ledMap[m_led];
//    m_opticsDriver.SetLedOff(nledChanIdx);
}
