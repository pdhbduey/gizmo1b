#include <LibLedBoardVersion2.h>

LibLedBoardVersion2::LibLedBoardVersion2() :
    m_led(SELECT_LED_BLUE1),
    m_ledThermistor(LibThermistor::NTCG163JF103FT1, LibThermistor::CELSIUS)
{
    m_ledToAdcMap[SELECT_LED_BLUE1] = LibAdcLedBoardVersion2::ADC_CHANNEL_0;
    m_ledToAdcMap[SELECT_LED_GREEN] = LibAdcLedBoardVersion2::ADC_CHANNEL_1;
    m_ledToAdcMap[SELECT_LED_RED1]  = LibAdcLedBoardVersion2::ADC_CHANNEL_2;
    m_ledToAdcMap[SELECT_LED_BROWN] = LibAdcLedBoardVersion2::ADC_CHANNEL_3;
    m_ledToAdcMap[SELECT_LED_RED2]  = LibAdcLedBoardVersion2::ADC_CHANNEL_4;
    m_ledToAdcMap[SELECT_LED_BLUE2] = LibAdcLedBoardVersion2::ADC_CHANNEL_5;
}

LibLedBoardVersion2::~LibLedBoardVersion2()
{
}

int LibLedBoardVersion2::setLed(uint32 led)
{
    if (m_ledToAdcMap.find(led) == m_ledToAdcMap.end()) {
        return ERROR_SELECT_LED_OUT_OF_RANGE;
    }
    m_led = led;
    return OKAY;
}

uint32 LibLedBoardVersion2::getLed()
{
    return m_led;
}

float LibLedBoardVersion2::readLedTemperature()
{
    uint32 data;
    m_libAdcLedBoardVersion2.read(m_ledToAdcMap[m_led], data);
    float rt = convertLedTemperatureRawDataToResistance(data);
    float temperature = m_ledThermistor.getTemperature(rt);
    return temperature;
}

// TEMP_AINx = Vref * Rt/(10,700 + Rt)
// Rt = 10,700 / (Vref / TEMP_AINx - 1)
float LibLedBoardVersion2::convertLedTemperatureRawDataToResistance(uint32 data)
{
    float vref    = m_libAdcLedBoardVersion2.getVref();
    float voltage = data  * vref / 65535;
    if (voltage == 0 || voltage == vref) {
        return 10000;
    }
    float rt = 10700 / (vref / voltage - 1);
    return rt;
}

uint32 LibLedBoardVersion2::getVersion()
{
    return LED_BOARD_V2;
}
