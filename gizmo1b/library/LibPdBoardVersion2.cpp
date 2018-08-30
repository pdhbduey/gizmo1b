#include <het.h>
#include <libWrapHet1.h>
#include <LibPdBoardVersion2.h>

LibPdBoardVersion2::LibPdBoardVersion2() :
    m_photodiode(SELECT_PHOTODIODE_1),
    m_integrationTimeInUs(10000),
    m_refV(4.096),
    m_photodiodeThermistorCurve(LibThermistorCurves::NTCG163JF103FT1, LibThermistorCurves::CELSIUS)
{
    m_pdToAdcMap[SELECT_PHOTODIODE_1] = 0;
    m_pdToAdcMap[SELECT_PHOTODIODE_2] = 1;
    m_pdToAdcMap[SELECT_PHOTODIODE_3] = 2;
    m_pdToAdcMap[SELECT_PHOTODIODE_4] = 3;
    m_pdToAdcMap[SELECT_PHOTODIODE_5] = 4;
    m_pdToAdcMap[SELECT_PHOTODIODE_6] = 5;

    m_pdTempToAdcMap[SELECT_PHOTODIODE_1] = 6;
    m_pdTempToAdcMap[SELECT_PHOTODIODE_2] = 7;
    m_pdTempToAdcMap[SELECT_PHOTODIODE_3] = 6;
    m_pdTempToAdcMap[SELECT_PHOTODIODE_4] = 7;
    m_pdTempToAdcMap[SELECT_PHOTODIODE_5] = 6;
    m_pdTempToAdcMap[SELECT_PHOTODIODE_6] = 7;

    m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_A] = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_28); // 107:HET1_28:DIG_OUT_B2
    m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_B] = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_30); // 127:HET1_30:DIG_OUT_B3
}

LibPdBoardVersion2::~LibPdBoardVersion2()
{
    delete m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_A];
    delete m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_B];
}

void LibPdBoardVersion2::setPhotodiode(uint32 photodiode)
{
    switch (photodiode) {
    case SELECT_PHOTODIODE_1:
    case SELECT_PHOTODIODE_2:
        m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_A]->m_libWrapGioPort->setPin(m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_A]->m_pin, false);
        m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_B]->m_libWrapGioPort->setPin(m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_B]->m_pin, false);
        break;
    case SELECT_PHOTODIODE_3:
    case SELECT_PHOTODIODE_4:
        m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_A]->m_libWrapGioPort->setPin(m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_A]->m_pin, true);
        m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_B]->m_libWrapGioPort->setPin(m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_B]->m_pin, false);
        break;
    case SELECT_PHOTODIODE_5:
    case SELECT_PHOTODIODE_6:
        m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_A]->m_libWrapGioPort->setPin(m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_A]->m_pin, false);
        m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_B]->m_libWrapGioPort->setPin(m_tempSwCCtrlPinsMap[TEMP_SW_CTRL_B]->m_pin, true);
        break;
    }
    m_photodiode = photodiode;
}

uint32 LibPdBoardVersion2::getPhotodiode()
{
    return m_photodiode;
}

float LibPdBoardVersion2::readPhotodiodeResult()
{
    uint32_t nledChanIdx   = 0;
    uint32_t npdChanIdx    = m_pdToAdcMap[m_photodiode];
    uint32_t nDuration_us  = m_integrationTimeInUs;
    uint32_t nLedIntensity = 0;
    m_photodiodeResultRaw = m_opticsDriverPd2.GetPhotoDiodeValue(nledChanIdx,
                                       npdChanIdx, nDuration_us, nLedIntensity);
    float result = m_photodiodeResultRaw * (m_refV / 65536);
    m_photodiodeTemperatureDuringIntegration = readPhotodiodeTemperature();
    return result;
}

uint32 LibPdBoardVersion2::readPhotodiodeResultRaw()
{
    return m_photodiodeResultRaw;
}

uint32 LibPdBoardVersion2::getVersion()
{
    return PHOTODIODE_BOARD_V2;
}

void LibPdBoardVersion2::setIntegrationTimeInUs(uint32 integrationTimeInUs)
{
    m_integrationTimeInUs = integrationTimeInUs;
}

uint32 LibPdBoardVersion2::getIntegrationTimeInUs()
{
    return m_integrationTimeInUs;
}

float LibPdBoardVersion2::readPhotodiodeTemperature()
{
    uint32_t nChanIdx = m_pdTempToAdcMap[m_photodiode];
    float raw = m_opticsDriverPd2.GetAdc(nChanIdx);
    float deg = convertRawToDeg(raw);
    return deg;
}

float LibPdBoardVersion2::readPhotodiodeTemperatureDuringIntegration()
{
    return m_photodiodeTemperatureDuringIntegration;
}

float LibPdBoardVersion2::convertRawToDeg(uint16_t data)
{
    float voltage = data  * m_refV / 65536;
    if (voltage == 0 || voltage == m_refV) {
        return 10000;
    }
    // TEMP_AINx = Vref * Rt/(10,700 + Rt)
    // Rt = 10,700 / (Vref / TEMP_AINx - 1)
    float rt = 10700 / (m_refV / voltage - 1);
    float deg =  m_photodiodeThermistorCurve.getTemperature(rt);
    return deg;
}
