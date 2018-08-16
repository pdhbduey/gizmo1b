#include <LibPdBoardVersion2.h>

LibPdBoardVersion2::LibPdBoardVersion2() :
    m_photodiode(SELECT_PHOTODIODE_D11_T1),
    m_integrationTimeInUs(10000),
    m_refV(4.096)
{
    m_pdToAdcMap[SELECT_PHOTODIODE_D11_T1] = 0;
    m_pdToAdcMap[SELECT_PHOTODIODE_D10_T1] = 1;
    m_pdToAdcMap[SELECT_PHOTODIODE_D11_T2] = 2;
    m_pdToAdcMap[SELECT_PHOTODIODE_D10_T2] = 3;
    m_pdToAdcMap[SELECT_PHOTODIODE_D11_T3] = 4;
    m_pdToAdcMap[SELECT_PHOTODIODE_D10_T3] = 5;
}

LibPdBoardVersion2::~LibPdBoardVersion2()
{
}

void LibPdBoardVersion2::setPhotodiode(uint32 photodiode)
{
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
    float result = m_photodiodeResultRaw * (m_refV / 65535);
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
