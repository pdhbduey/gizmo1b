#include "mibspi.h"
#include "libWrapMibSpi5.h"
#include "libMutex.h"
#include "libTec.h"

SemaphoreHandle_t LibTec::s_mutex;
bool LibTec::s_isInitialized;

LibTec::LibTec() :
    m_tecEnable(new LibWrapMibSpi5, PIN_SIMO), // 99:MIBSPI5SIMO[0]:TEC_EN
    m_waveformPeriod(1000)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        enable(false);
        s_isInitialized = true;
    }
}

LibTec::~LibTec()
{
}

void LibTec::enable(bool en)
{
    LibMutex libMutex(s_mutex);
    m_tecEnable.m_libWrapGioPort->setPin(m_tecEnable.m_pin, en);
}

// TEC_ISENSE = (ISENSE * 0.008ohm * 20V/V + 2.5V) * 6.04K/9.05K
int LibTec::getISense(float& iSense)
{
    LibMutex libMutex(s_mutex);
    float value;
    int result = m_libAdc.read(ISENSE, value);
    if (result != LibAdc::OKAY) {
        return ERROR_ISENSE;
    }

    iSense = (value * 9.05 / 6.04 - 2.5) / (20.0 * 0.008);
    return OKAY;
}

// TEC_VSENSE = VSENSE * 1.5K/21.5K + 2.5V
int LibTec::getVSense(float& vSense)
{
    LibMutex libMutex(s_mutex);
    float value;
    int result = m_libAdc.read(VSENSE, value);
    if (result != LibAdc::OKAY) {
        return ERROR_VSENSE;
    }
    vSense = (value - 2.5) * 21.5 / 1.5;
    return OKAY;
}

int LibTec::setRefCurrent(float refCurrent)
{
    LibMutex libMutex(s_mutex);
    if (refCurrent < -15.0 && refCurrent > 15.0) {
        return ERROR_REF_CURRENT_OUT_OF_RANGE;
    }
    float value = refCurrent / 15.0 * 2.5 + 2.5;
    int result = m_libDac.set(value);
    if (result != LibDac::OKAY) {
        return ERROR_SET_REF_CURRENT;
    }
    return OKAY;
}

float LibTec::getRefCurrent()
{
    LibMutex libMutex(s_mutex);
    float value = m_libDac.get() ;
    float refCurrent = (value - 2.5) / 2.5 * 15.0;
    return refCurrent;
}

int LibTec::setWaveformType(uint32 waveformType)
{
    int status = OKAY;
    LibMutex libMutex(s_mutex);
    switch (waveformType) {
    default:
        status = ERROR_WAVEFORM_TYPE_OUT_OF_RANGE;
        break;
    case WAVEFORM_TYPE_CONSTANT:
    case WAVEFORM_TYPE_SINUSOIDAL:
    case WAVEFORM_TYPE_TRIANGULAR:
    case WAVEFORM_TYPE_SQUARE:
        break;
    }
    if (status == OKAY) {
        m_waveformType = waveformType;
    }
    return status;
}

uint32 LibTec::getWaveformType()
{
    LibMutex libMutex(s_mutex);
    return m_waveformType;
}

int LibTec::setWaveformPeriod(uint32 waveformPeriod)
{
    int status = OKAY;
    LibMutex libMutex(s_mutex);
    if (waveformPeriod < 500 || waveformPeriod > 10000) {
        status = ERROR_WAVEFORM_PERIOD_OUT_OF_RANGE;
    }
    if (status == OKAY) {
        m_waveformPeriod = waveformPeriod;
    }
    return status;
}

uint32 LibTec::getWaveformPeriod()
{
    LibMutex libMutex(s_mutex);
    return m_waveformPeriod;
}

void LibTec::waveformStart()
{
    // Generate waveform
    // Start waveform thread
}

void LibTec::waveformStop()
{
    // Terminate waveform thread
    // Set Iref = 0
}
