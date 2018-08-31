#include <het.h>
#include <libWrapHet1.h>
#include <LibDelay.h>
#include <LibLedBoardVersion2.h>

LibLedBoardVersion2::LibLedBoardVersion2() :
    m_led(SELECT_LED_1),
    m_ledThermistorCurve(LibThermistorCurves::NTCG163JF103FT1, LibThermistorCurves::CELSIUS),
    m_ledIntensity(0),
    m_refV(4.096)
{
    m_ledTempToAdcMap[SELECT_LED_1] = 0;
    m_ledTempToAdcMap[SELECT_LED_2] = 1;
    m_ledTempToAdcMap[SELECT_LED_3] = 2;
    m_ledTempToAdcMap[SELECT_LED_4] = 3;
  //m_ledTempToAdcMap[SELECT_LED_5] = 4; // (Not on BB)
  //m_ledTempToAdcMap[SELECT_LED_6] = 5; // (Not on BB)

    m_ledMonPdAdcChannel   = 6;
    m_ledVisenseAdcChannel = 7;

    m_ledToDacMap[SELECT_LED_1] = OpticsDriver::kBlue1;
    m_ledToDacMap[SELECT_LED_2] = OpticsDriver::kGreen;
    m_ledToDacMap[SELECT_LED_3] = OpticsDriver::kRed1;
    m_ledToDacMap[SELECT_LED_4] = OpticsDriver::kBrown;
  //m_ledToDacMap[SELECT_LED_5] = OpticsDriver::kRed2;  // (Not on BB)
  //m_ledToDacMap[SELECT_LED_6] = OpticsDriver::kBlue2; // (Not on BB)

    m_ledCtrlSwPinsMap[LED_CTRL_S0] = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_15); // 41:HET1_15:DIG_OUT_A3
    m_ledCtrlSwPinsMap[LED_CTRL_S1] = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_13); // 39:HET1_13:DIG_OUT_A1
  //m_ledCtrlSwPinsMap[LED_CTRL_S2] = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_XX); // GND (Not on BB)
}

LibLedBoardVersion2::~LibLedBoardVersion2()
{
}

void LibLedBoardVersion2::setLed(uint32 led)
{
    switch (led) {
    default:
    case SELECT_LED_1:
        m_ledCtrlSwPinsMap[LED_CTRL_S0]->m_libWrapGioPort->setPin(m_ledCtrlSwPinsMap[LED_CTRL_S0]->m_pin, false);
        m_ledCtrlSwPinsMap[LED_CTRL_S1]->m_libWrapGioPort->setPin(m_ledCtrlSwPinsMap[LED_CTRL_S1]->m_pin, false);
        break;
    case SELECT_LED_2:
        m_ledCtrlSwPinsMap[LED_CTRL_S0]->m_libWrapGioPort->setPin(m_ledCtrlSwPinsMap[LED_CTRL_S0]->m_pin, true);
        m_ledCtrlSwPinsMap[LED_CTRL_S1]->m_libWrapGioPort->setPin(m_ledCtrlSwPinsMap[LED_CTRL_S1]->m_pin, false);
        break;
    case SELECT_LED_3:
        m_ledCtrlSwPinsMap[LED_CTRL_S0]->m_libWrapGioPort->setPin(m_ledCtrlSwPinsMap[LED_CTRL_S0]->m_pin, false);
        m_ledCtrlSwPinsMap[LED_CTRL_S1]->m_libWrapGioPort->setPin(m_ledCtrlSwPinsMap[LED_CTRL_S1]->m_pin, true);
        break;
    case SELECT_LED_4:
        m_ledCtrlSwPinsMap[LED_CTRL_S0]->m_libWrapGioPort->setPin(m_ledCtrlSwPinsMap[LED_CTRL_S0]->m_pin, true);
        m_ledCtrlSwPinsMap[LED_CTRL_S1]->m_libWrapGioPort->setPin(m_ledCtrlSwPinsMap[LED_CTRL_S1]->m_pin, true);
        break;
    }
    m_led = led;
}

uint32 LibLedBoardVersion2::getLed()
{
    return m_led;
}

float LibLedBoardVersion2::readPhotodiodeResult(uint32_t integrationTimeInUs)
{
    if (m_ledTempToAdcMap.find(m_led) == m_ledTempToAdcMap.end()) {
        m_ledMonPdDuringIntegration       = 0;
        m_ledTemperatureDuringIntegration = 0;
        return 0;
    }
    uint32_t nLedIntensity = m_ledIntensity;
    m_opticsDriverLed2.SetLedIntensity(0, nLedIntensity);
    LibDelay::pmuMicrosecDelay(integrationTimeInUs);
    m_ledMonPdDuringIntegration = readLedMonitorPhotodiode();
    m_ledTemperatureDuringIntegration = readLedTemperature();
    m_opticsDriverLed2.SetLedsOff();
    return 0;
}

float LibLedBoardVersion2::readLedMonitorPhotodiode()
{
    uint32_t nChanIdx = m_ledMonPdAdcChannel;
    float raw = m_opticsDriverLed2.GetAdc(nChanIdx);
    float result = raw * (m_refV / 65536);
    return result;
}

float LibLedBoardVersion2::readLedMonitorPhotodiodeDuringIntegration()
{
    return m_ledMonPdDuringIntegration;
}

float LibLedBoardVersion2::readLedTemperature()
{
    if (m_ledTempToAdcMap.find(m_led) == m_ledTempToAdcMap.end()) {
        return 0;
    }
    uint32_t nChanIdx = m_ledTempToAdcMap[m_led];
    float raw = m_opticsDriverLed2.GetAdc(nChanIdx);
    float deg = convertRawToDeg(raw);
    return deg;
}

float LibLedBoardVersion2::readLedTemperatureDuringIntegration()
{
    return m_ledTemperatureDuringIntegration;
}

float LibLedBoardVersion2::convertRawToDeg(uint32 data)
{
    float voltage = data  * m_refV / 65536;
    if (voltage == 0 || voltage == m_refV) {
        return 10000;
    }
    // TEMP_AINx = Vref * Rt/(10,700 + Rt)
    // Rt = 10,700 / (Vref / TEMP_AINx - 1)
    float rt = 10700 / (m_refV / voltage - 1);
    float deg =  m_ledThermistorCurve.getTemperature(rt);
    return deg;
}

uint32 LibLedBoardVersion2::getVersion()
{
    return LED_BOARD_V2;
}

void LibLedBoardVersion2::setLedIntensity(uint32 ledIntensity)
{
    m_ledIntensity = ledIntensity;
}

uint32 LibLedBoardVersion2::getLedIntensity()
{
    return m_ledIntensity;
}

void LibLedBoardVersion2::turnLedOn()
{
    if (m_ledTempToAdcMap.find(m_led) == m_ledTempToAdcMap.end()) {
        return;
    }
    uint32_t nLedIntensity = m_ledIntensity;
    m_opticsDriverLed2.SetLedIntensity(0, nLedIntensity);
}

void LibLedBoardVersion2::turnLedOff()
{
    if (m_ledTempToAdcMap.find(m_led) == m_ledTempToAdcMap.end()) {
        return;
    }
    m_opticsDriverLed2.SetLedIntensity(0, 0);
}
