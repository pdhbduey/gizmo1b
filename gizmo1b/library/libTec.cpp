#include <math.h>
#include "mibspi.h"
#include "libWrapMibSpi5.h"
#include "libMutex.h"
#include "libTec.h"

SemaphoreHandle_t LibTec::s_mutex;
SemaphoreHandle_t LibTec::s_sem;
bool LibTec::s_isInitialized;

LibTec::LibTec(const char* name) :
    LibTask(name, configMINIMAL_STACK_SIZE, configMAX_PRIORITIES - 1),
    m_tecEnable(new LibWrapMibSpi5, PIN_SIMO), // 99:MIBSPI5SIMO[0]:TEC_EN
    m_waveformPeriod(1000)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_sem = xSemaphoreCreateBinary();
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

int LibTec::driveReference(float refCurrent)
{
    float ref = refCurrent / 15.0 * 2.5 + 2.5;
    int result = m_libDac.set(ref);
    return result != LibDac::OKAY ? ERROR_SET_REF_CURRENT : OKAY;
}

int LibTec::setRefCurrent(float refCurrent)
{
    LibMutex libMutex(s_mutex);
    if (refCurrent < -15.0 && refCurrent > 15.0) {
        return ERROR_REF_CURRENT_OUT_OF_RANGE;
    }
    m_refCurrent = refCurrent;
    if (m_waveformType == WAVEFORM_TYPE_CONSTANT)  {
        return driveReference(m_refCurrent);
    }
    return OKAY;
}

float LibTec::getRefCurrent()
{
    LibMutex libMutex(s_mutex);
    if (m_waveformType == WAVEFORM_TYPE_CONSTANT)  {
        float value = m_libDac.get() ;
        m_refCurrent = (value - 2.5) / 2.5 * 15.0;
    }
    return m_refCurrent;
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
    if (waveformPeriod < 1 || waveformPeriod > 10) {
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
    LibMutex libMutex(s_mutex);
    if (m_waveformType == WAVEFORM_TYPE_CONSTANT || m_isWaveformRunning)  {
        return;
    }
    TickType_t ticksPeriod = pdMS_TO_TICKS(m_waveformPeriod * 1000);
    TickType_t ticksStep   = ticksPeriod / 1000;
    switch (m_waveformType) {
    default:
        return;
    case WAVEFORM_TYPE_SINUSOIDAL:
        for (int i = 0; i < 1000; i++) {
            TickType_t ticks = ticksStep * i;
            float value = sin(2 * 3.141 * ticks / ticksPeriod);
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_value = value;
        }
        break;
    case WAVEFORM_TYPE_TRIANGULAR:
        for (int i = 0; i < 250; i++) {
            TickType_t ticks = ticksStep * i;
            float value = 4.0 * ticks / ticksPeriod;
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_value = value;
        }
        for (int i = 250; i < 750; i++) {
            TickType_t ticks = ticksStep * i;
            float value = 2.0 - 4.0 * ticks / ticksPeriod;
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_value = value;
        }
        for (int i = 750; i < 1000; i++) {
            TickType_t ticks = ticksStep * i;
            float value = 4.0 * ticks / ticksPeriod - 4.0;
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_value = value;
        }
        break;
    case WAVEFORM_TYPE_SQUARE:
        for (int i = 0; i < 500; i++) {
            TickType_t ticks = ticksStep * i;
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_value = 1;
        }
        for (int i = 500; i < 1000; i++) {
            TickType_t ticks = ticksStep * i;
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_value = -1;
        }
        break;
    }
    m_ticks = 0;
    m_isWaveformRunning = true;
    if (!m_isTaskRunning) {
        m_isTaskRunning = true;
        start();
    }
    xSemaphoreGive(s_sem);
}

void LibTec::waveformStop()
{
    LibMutex libMutex(s_mutex);
    if (m_waveformType == WAVEFORM_TYPE_CONSTANT || !m_isWaveformRunning)  {
        return;
    }
    m_isWaveformRunning = false;
    xSemaphoreGive(s_sem);
}

void LibTec::run()
{
    TickType_t ticksToWait = portMAX_DELAY;
    while (true) {
        xSemaphoreTake(s_sem, ticksToWait);
        {
            LibMutex libMutex(s_mutex);
            if (!m_isWaveformRunning) {
                ticksToWait = portMAX_DELAY;
                driveReference(m_refCurrent);
                continue;
            }
            else {
                ticksToWait = 1;
            }
        }
        //driveReference(0); // debug
        TickType_t ticks = xTaskGetTickCount() - m_ticks;
        if (m_waveform[999].m_ticks < ticks) {
            m_ticks = xTaskGetTickCount();
            ticks   = 0;
        }
        for (int i = 0; i < 1000; i++) {
            if (ticks >= m_waveform[i].m_ticks
             && ticks <= m_waveform[i + 1].m_ticks) {
                float value = m_waveform[i].m_value
                            + (m_waveform[i + 1].m_value - m_waveform[i].m_value)
                            * (ticks - m_waveform[i].m_ticks)
                            / (m_waveform[i + 1].m_ticks - m_waveform[i].m_ticks);
                driveReference(m_refCurrent * value);
                break;
            }
        }
        //driveReference(m_refCurrent); // debug
    }
}
