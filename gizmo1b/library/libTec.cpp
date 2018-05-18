#include <cmath>
#include <cstdlib>
#include "mibspi.h"
#include "libWrapMibSpi5.h"
#include "libMutex.h"
#include "libTec.h"

SemaphoreHandle_t LibTec::s_mutex;
bool LibTec::s_isInitialized;
float* LibTec::s_snapshotVsense;
float* LibTec::s_snapshotIsense;
float* LibTec::s_snapshotIref;
float* LibTec::s_snapshotT1;
float* LibTec::s_snapshotT2;
float* LibTec::s_snapshotT3;
float* LibTec::s_snapshotT4;
SemaphoreHandle_t LibTec::s_traceMutex;

void LibTec::IrefSample::clear()
{
    m_time = 0;
    m_iref = 0;
}

LibTec::LibTec(const char* name) :
    LibTask(name, configMINIMAL_STACK_SIZE, configMAX_PRIORITIES - 1),
    m_tecEnable(new LibWrapMibSpi5, PIN_SIMO), // 99:MIBSPI5SIMO[0]:TEC_EN
    m_waveformPeriod(1000),
    m_pidProportionalGain(1),
    m_snapshotNumSamples(10),
    m_snapshotRes(SNAPSHOT_RES_10),
    m_traceCircularBuffer(1000)
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        m_tecEnable.m_libWrapGioPort->setPin(m_tecEnable.m_pin, false);
        s_snapshotVsense = new float[1000];
        s_snapshotIsense = new float[1000];
        s_snapshotIref   = new float[1000];
        s_snapshotT1     = new float[1000];
        s_snapshotT2     = new float[1000];
        s_snapshotT3     = new float[1000];
        s_snapshotT4     = new float[1000];
        s_traceMutex    = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
    // LP FIR filter taps are created in Matlab with the following command:
    // firls(20,[0 0.2 0.25 1],[1 1 0 0], [100 1])
    m_filterTaps.push_back( 0.007794814985338);
    m_filterTaps.push_back( 0.026552774207396);
    m_filterTaps.push_back( 0.011742309467546);
    m_filterTaps.push_back(-0.021356465458395);
    m_filterTaps.push_back(-0.048362325215853);
    m_filterTaps.push_back(-0.046379464057561);
    m_filterTaps.push_back(-0.004390272701631);
    m_filterTaps.push_back( 0.071104494811224);
    m_filterTaps.push_back( 0.157852120303661);
    m_filterTaps.push_back( 0.226497563253852);
    m_filterTaps.push_back( 0.252541521407335);
    m_filterTaps.push_back( 0.226497563253852);
    m_filterTaps.push_back( 0.157852120303661);
    m_filterTaps.push_back( 0.071104494811224);
    m_filterTaps.push_back(-0.004390272701631);
    m_filterTaps.push_back(-0.046379464057561);
    m_filterTaps.push_back(-0.048362325215853);
    m_filterTaps.push_back(-0.021356465458395);
    m_filterTaps.push_back( 0.011742309467546);
    m_filterTaps.push_back( 0.026552774207396);
    m_filterTaps.push_back( 0.007794814985338);
    for (int i = 0; i < m_filterTaps.size(); i++) {
        m_filterQueue.push(0);
    }
}

LibTec::~LibTec()
{
}

void LibTec::enable(bool en)
{
    LibMutex libMutex(s_mutex);
    m_tecEnable.m_libWrapGioPort->setPin(m_tecEnable.m_pin, en);
    m_isEnabled = en;
}

bool LibTec::isEnabled()
{
    LibMutex libMutex(s_mutex);
    return m_isEnabled;
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

bool LibTec::driveControl(float control)
{
    float ref = control + 2.5;
    ref = ref > 5 ? 5 : ref < 0 ? 0 : ref; // limit to [0,5]
    int result = m_libDac.set(ref);
    return result != LibDac::OKAY ? ERROR_SET_REF_CURRENT : OKAY;
}

bool LibTec::isRefCurrentValid(float refCurrent)
{
    return refCurrent >= -15.0 && refCurrent <= 15.0;
}

int LibTec::setRefCurrent(float refCurrent)
{
    LibMutex libMutex(s_mutex);
    if (!isRefCurrentValid(refCurrent)) {
        return ERROR_REF_CURRENT_OUT_OF_RANGE;
    }
    m_refCurrent = refCurrent;
    return OKAY;
}

float LibTec::getRefCurrent()
{
    LibMutex libMutex(s_mutex);
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
    case WAVEFORM_TYPE_CUSTOM:
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

bool LibTec::isWaveformPeriodValid(uint32 waveformPeriod)
{
    return 2 <= waveformPeriod && waveformPeriod <= 10000;
}

int LibTec::setWaveformPeriod(uint32 waveformPeriod)
{
    int status = OKAY;
    LibMutex libMutex(s_mutex);
    if (!isWaveformPeriodValid(waveformPeriod)) {
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

float LibTec::getIrefFromCustomWaveform(std::vector<struct IrefSample>& waveform)
{
    float iref = waveform[0].m_iref;
    for (int i = 1; i < waveform.size(); i++) {
        if (std::abs(waveform[i].m_iref) > std::abs(iref)) {
            iref = waveform[i].m_iref;
        }
    }
    return iref;
}

uint32 LibTec::getPeriodFromCustomWaveform(std::vector<struct IrefSample>& waveform)
{
    return waveform[waveform.size() - 1].m_time + 1;
}

bool LibTec::isCustomWaveformEmpty(std::vector<struct IrefSample>& waveform)
{
    return waveform.size() == 0;
}

bool LibTec::isCustomWaveformStartTimeZero(std::vector<struct IrefSample>& waveform)
{
    return waveform[0].m_time == 0;
}

bool LibTec::isCustomWaveformTimeRising(std::vector<struct IrefSample>& waveform)
{
    if (waveform.size() > 1) {
        for (int i = 0; i < waveform.size() - 1; i++) {
            if (waveform[i].m_time >= waveform[i + 1].m_time) {
                return false;
            }
        }
    }
    return true;
}

int LibTec::setCustomWaveform(std::vector<struct IrefSample>& waveform, uint32 cycles)
{
    if (isCustomWaveformEmpty(waveform)) {
        return ERROR_CUSTOM_WAVEFORM_EMPTY;
    }
    if (!isCustomWaveformStartTimeZero(waveform)) {
        return ERROR_CUSTOM_WAVEFORM_NON_ZERO_START_TIME;
    }
    if (!isCustomWaveformTimeRising(waveform)) {
        return ERROR_CUSTOM_WAVEFORM_TIME_NOT_RISING;
    }
    uint32 waveformPeriod = getPeriodFromCustomWaveform(waveform);
    if (!isWaveformPeriodValid(waveformPeriod)) {
        return ERROR_WAVEFORM_PERIOD_OUT_OF_RANGE;
    }
    float iref = getIrefFromCustomWaveform(waveform);
    if (!isRefCurrentValid(iref)) {
        return ERROR_REF_CURRENT_OUT_OF_RANGE;;
    }
    LibMutex libMutex(s_mutex);
    m_customWaveform.clear();
    m_customWaveform = waveform;
    m_cycles = cycles;
    return OKAY;
}

void LibTec::getCustomWaveform(std::vector<struct IrefSample>& waveform, uint32& cycles)
{
    waveform.clear();
    waveform = m_customWaveform;
    cycles = m_cycles;
}

void LibTec::waveformStart()
{
    LibMutex libMutex(s_mutex);
    if (m_waveformType == WAVEFORM_TYPE_CONSTANT || m_isWaveformRunning)  {
        return;
    }
    float ticksPeriod = pdMS_TO_TICKS(m_waveformPeriod);
    float ticksStep = ticksPeriod / 1000;
    switch (m_waveformType) {
    default:
        return;
    case WAVEFORM_TYPE_SINUSOIDAL:
        for (int i = 0; i < 1000; i++) {
            float ticks = ticksStep * i;
            float value = std::sinf(i * 2 * 3.141 / 1000);
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_iref = value;
        }
        break;
    case WAVEFORM_TYPE_TRIANGULAR:
        for (int i = 0; i < 250; i++) {
            float ticks = ticksStep * i;
            float value = 4.0 * ticks / ticksPeriod;
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_iref = value;
        }
        for (int i = 250; i < 750; i++) {
            float ticks = ticksStep * i;
            float value = 2.0 - 4.0 * ticks / ticksPeriod;
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_iref = value;
        }
        for (int i = 750; i < 1000; i++) {
            float ticks = ticksStep * i;
            float value = 4.0 * ticks / ticksPeriod - 4.0;
            m_waveform[i].m_ticks = ticks;
            m_waveform[i].m_iref = value;
        }
        break;
    case WAVEFORM_TYPE_SQUARE:
        break;
    case WAVEFORM_TYPE_CUSTOM:
        if (!isCustomWaveformEmpty(m_customWaveform) || m_customWaveform.size() > 1) {
            uint32 waveformPeriod = getPeriodFromCustomWaveform(m_customWaveform);
            ticksPeriod = pdMS_TO_TICKS(waveformPeriod);
            ticksStep = ticksPeriod / 1000;
            for (int i = 0; i < 1000; i++) {
                float ticks = ticksStep * i;
                m_waveform[i].m_ticks = ticks;
                m_waveform[i].m_iref = -1000; // to invalidate the iref
            }
            for (int i = 0, j = 0; i < 999; i++) {
                if (j < m_customWaveform.size()) {
                    float leftTimeTicks = m_customWaveform[j].m_time
                                                  * configTICK_RATE_HZ / 1000.0;
                    if (m_waveform[i].m_ticks <= leftTimeTicks
                     && leftTimeTicks <= m_waveform[i + 1].m_ticks) {
                        m_waveform[i].m_iref = m_customWaveform[j++].m_iref;
                    }
                }
            }
            for (int i = 0, j = 1; i < m_customWaveform.size() - 1; i++) {
                struct IrefSample irefSampleLeft = m_customWaveform[i];
                struct IrefSample irefSampleRight = m_customWaveform[i + 1];
                float dticks = (irefSampleRight.m_time - irefSampleLeft.m_time)
                                                  * configTICK_RATE_HZ / 1000.0;
                float dy = irefSampleRight.m_iref - irefSampleLeft.m_iref;
                float leftTimeTicks = irefSampleLeft.m_time
                                                  * configTICK_RATE_HZ / 1000.0;
                for ( ; j < 1000; j++) {
                    float ticks = ticksStep * j;
                    float iref = irefSampleLeft.m_iref
                                        + dy / dticks * (ticks - leftTimeTicks);
                    if (m_waveform[j].m_iref == irefSampleRight.m_iref) {
                        j++;
                        break;
                    }
                    m_waveform[j].m_ticks = ticks;
                    m_waveform[j].m_iref = iref;
                }
            }
        }
        break;
    }
    m_ticks = 0;
    m_isWaveformRunning = true;
}

void LibTec::waveformStop()
{
    LibMutex libMutex(s_mutex);
    if (m_waveformType == WAVEFORM_TYPE_CONSTANT || !m_isWaveformRunning)  {
        return;
    }
    m_isWaveformRunning = false;
}

bool LibTec::isWaveformStarted()
{
    LibMutex libMutex(s_mutex);
    return m_isWaveformRunning;
}

void LibTec::closedLoopEnable()
{
    LibMutex libMutex(s_mutex);
    m_isClosedLoopInitialized = false;
    m_isClosedLoopEnabled = true;
}

void LibTec::closedLoopDisable()
{
    LibMutex libMutex(s_mutex);
    m_isClosedLoopEnabled = false;
}

int LibTec::setProportionalGain(float gain)
{
    LibMutex libMutex(s_mutex);
    if (gain < 0.01 || gain > 100) {
        return ERROR_PROPORTIONAL_GAIN_OUT_OF_RANGE;
    }
    m_pidProportionalGain = gain;
    return OKAY;
}

float LibTec::getProportionalGain()
{
    LibMutex libMutex(s_mutex);
    return m_pidProportionalGain;
}

int LibTec::setIntegralGain(float gain)
{
    LibMutex libMutex(s_mutex);
    if (gain < 0 || gain > 100) {
        return ERROR_INTEGRAL_GAIN_OUT_OF_RANGE;
    }
    m_pidIntegralGain = gain;
    return OKAY;
}

float LibTec::getIntegrallGain()
{
    LibMutex libMutex(s_mutex);
    return m_pidIntegralGain;
}

int LibTec::setDerivativeGain(float gain)
{
    LibMutex libMutex(s_mutex);
    if (gain < 0 || gain > 100) {
        return ERROR_DERIVATIVE_GAIN_OUT_OF_RANGE;
    }
    m_pidDerivativeGain = gain;
    return OKAY;
}

float LibTec::getDerivativeGain()
{
    LibMutex libMutex(s_mutex);
    return m_pidDerivativeGain;
}

bool LibTec::isClosedLoopEnabled()
{
    LibMutex libMutex(s_mutex);
    return m_isClosedLoopEnabled;
}

float LibTec::filter(float value)
{
    std::queue<float> temp;
    m_filterQueue.push(value);
    m_filterQueue.pop();
    float acc = 0;
    for (int i = 0; i < m_filterTaps.size(); i++) {
        float value = m_filterQueue.front();
        float tap   = m_filterTaps[i];
        acc += m_filterTaps[i] * m_filterQueue.front();
        temp.push(m_filterQueue.front());
        m_filterQueue.pop();
    }
    for (int i = 0; i < m_filterTaps.size(); i++) {
        m_filterQueue.push(temp.front());
        temp.pop();
    }
    return acc;
}

float LibTec::getWaveformSample(TickType_t tick)
{
    float value = m_refCurrent;
    if (m_isWaveformRunning) {
        TickType_t ticks = tick - m_ticks;
        if (m_waveformType == WAVEFORM_TYPE_SQUARE)  {
            float ticksPeriod = pdMS_TO_TICKS(m_waveformPeriod);
            if (ticks > ticksPeriod) {
                m_ticks = xTaskGetTickCount();
                m_refCurrent = -m_refCurrent;
            }
            else if (ticks < ticksPeriod / 2 && ticks + 1 >= ticksPeriod / 2) {
                m_refCurrent = -m_refCurrent;
            }
            return m_refCurrent;
        }
        if (m_waveform[999].m_ticks < ticks) {
            m_ticks = xTaskGetTickCount();
            ticks   = 0;
        }
        for (int i = 0; i < 1000; i++) {
            if (ticks >= m_waveform[i].m_ticks
             && ticks <= m_waveform[i + 1].m_ticks) {
                value = m_waveform[i].m_iref
                      + (m_waveform[i + 1].m_iref - m_waveform[i].m_iref)
                      * (ticks - m_waveform[i].m_ticks)
                      / (m_waveform[i + 1].m_ticks - m_waveform[i].m_ticks);
                break;
            }
        }
        if (m_waveformType != WAVEFORM_TYPE_CUSTOM) {
           value *= m_refCurrent;
        }
    }
    return value;
}

void LibTec::run()
{
    while (true) {
        vTaskDelay(1);
        TickType_t tick = xTaskGetTickCount();
        float iRef      = getWaveformSample(tick);
        float pidError  = iRef;
        float iSense    = 0;
        if (m_isClosedLoopEnabled && getISense(iSense) == OKAY) {
            pidError -= iSense;
        }
        pidError = filter(pidError);
        if (!m_isClosedLoopInitialized || !m_isEnabled) {
            m_prevError = pidError;
            m_accError = 0;
            if (!m_isClosedLoopInitialized) {
                m_isClosedLoopInitialized = true;
            }
        }
        if (m_pidDerivativeGain == 0) {
            m_prevError = pidError;
        }
        if (m_pidIntegralGain == 0) {
            m_accError = 0;
        }
        float control = m_pidProportionalGain *  pidError
                      + m_pidIntegralGain     *  m_accError
                      + m_pidDerivativeGain   * (pidError - m_prevError);
        m_prevError   = pidError;
        m_accError   += pidError;
        control      *= 2.5 / 15;
        if (m_isEnabled) {
            driveControl(control);
        }
        if (m_isSnapshotRunning) {
            int res[3] = { [SNAPSHOT_RES_10  ] = 100,
                           [SNAPSHOT_RES_100 ] =  10,
                           [SNAPSHOT_RES_1000] =   1, };
            if (tick % res[m_snapshotRes]) {
                continue;
            }
            if (m_snapshotSample < m_snapshotNumSamples) {
                getVSense(s_snapshotVsense[m_snapshotSample]);
                s_snapshotIsense[m_snapshotSample] = iSense;
                s_snapshotIref  [m_snapshotSample] = iRef;
                m_libThermistor.readTemp(LibThermistor::AIN_A, s_snapshotT1[m_snapshotSample]);
                m_libThermistor.readTemp(LibThermistor::AIN_B, s_snapshotT2[m_snapshotSample]);
                m_libThermistor.readTemp(LibThermistor::AIN_C, s_snapshotT3[m_snapshotSample]);
                m_libThermistor.readTemp(LibThermistor::AIN_D, s_snapshotT4[m_snapshotSample]);
                m_snapshotSample++;
            }
            else {
                m_isSnapshotRunning = false;
            }
        }
        else if (m_isTraceRunning) {
            LibMutex libMutex(s_traceMutex);
            int res[3] = { [TRACE_RES_10 ] = 100,
                           [TRACE_RES_100] =  10, };
            if (tick % res[m_traceRes]) {
                continue;
            }
            int traceSample = m_traceCircularBuffer.back();
            getVSense(s_snapshotVsense[traceSample]);
            s_snapshotIsense[traceSample] = iSense;
            s_snapshotIref  [traceSample] = iRef;
            m_libThermistor.readTemp(LibThermistor::AIN_A, s_snapshotT1[traceSample]);
            m_libThermistor.readTemp(LibThermistor::AIN_B, s_snapshotT2[traceSample]);
            m_libThermistor.readTemp(LibThermistor::AIN_C, s_snapshotT3[traceSample]);
            m_libThermistor.readTemp(LibThermistor::AIN_D, s_snapshotT4[traceSample]);
            m_traceCircularBuffer.pushBack();
        }
    }
}

void LibTec::startSnaphot()
{
    LibMutex libMutex(s_mutex);
    if (!m_isSnapshotRunning) {
        m_snapshotSample    = 0;
        m_isSnapshotRunning = true;
    }
}

void LibTec::stopSnapshot()
{
    LibMutex libMutex(s_mutex);
    if (m_isSnapshotRunning) {
        m_isSnapshotRunning = false;
    }
}

int LibTec::setSnapshotNumberOfSamples(int nsamples)
{
    LibMutex libMutex(s_mutex);
    int result = OKAY;
    if (10 <= nsamples && nsamples <= 1000) {
        m_snapshotNumSamples = nsamples;
    }
    else {
        result = ERROR_SNAPSHOT_NUMBER_OF_SAMPLES_OUT_OF_RANGE;
    }
    return result;
}

int LibTec::getSnapshotNumberOfSamples()
{
    return m_snapshotNumSamples;
}

int LibTec::setSnapshotResolution(int res)
{
    LibMutex libMutex(s_mutex);
    int result = OKAY;
    switch (res) {
    default:
        result = ERROR_SNAPSHOT_RESOLUTION_OUT_OF_RANGE;
        break;
    case SNAPSHOT_RES_10:
    case SNAPSHOT_RES_100:
    case SNAPSHOT_RES_1000:
        m_snapshotRes = res;
        break;
    }
    return result;
}

int LibTec::getSnapshotResolution()
{
    return m_snapshotRes;
}

bool LibTec::isSnapshotRunning()
{
    return m_isSnapshotRunning;
}

bool LibTec::isSnapshotSampleInRange(int sample)
{
    return (0 <= sample && sample <= 999);
}

int LibTec::getSnapshotVsense(int sample, float& value)
{
    int result = OKAY;
    if (isSnapshotSampleInRange(sample)) {
        value = s_snapshotVsense[sample];
    }
    else {
        result = ERROR_SNAPSHOT_SAMPLE_OUT_OF_RANGE;
    }
    return result;
}

int LibTec::getSnapshotIsense(int sample, float& value)
{
    int result = OKAY;
    if (isSnapshotSampleInRange(sample)) {
        value = s_snapshotIsense[sample];
    }
    else {
        result = ERROR_SNAPSHOT_SAMPLE_OUT_OF_RANGE;
    }
    return result;
}

int LibTec::getSnapshotIref(int sample, float& value)
{
    int result = OKAY;
    if (isSnapshotSampleInRange(sample)) {
        value = s_snapshotIref[sample];
    }
    else {
        result = ERROR_SNAPSHOT_SAMPLE_OUT_OF_RANGE;
    }
    return result;
}

int LibTec::getSnapshotT1(int sample, float& value)
{
    int result = OKAY;
    if (isSnapshotSampleInRange(sample)) {
        value = s_snapshotT1[sample];
    }
    else {
        result = ERROR_SNAPSHOT_SAMPLE_OUT_OF_RANGE;
    }
    return result;
}

int LibTec::getSnapshotT2(int sample, float& value)
{
    int result = OKAY;
    if (isSnapshotSampleInRange(sample)) {
        value = s_snapshotT2[sample];
    }
    else {
        result = ERROR_SNAPSHOT_SAMPLE_OUT_OF_RANGE;
    }
    return result;
}

int LibTec::getSnapshotT3(int sample, float& value)
{
    int result = OKAY;
    if (isSnapshotSampleInRange(sample)) {
        value = s_snapshotT3[sample];
    }
    else {
        result = ERROR_SNAPSHOT_SAMPLE_OUT_OF_RANGE;
    }
    return result;
}

int LibTec::getSnapshotT4(int sample, float& value)
{
    int result = OKAY;
    if (isSnapshotSampleInRange(sample)) {
        value = s_snapshotT4[sample];
    }
    else {
        result = ERROR_SNAPSHOT_SAMPLE_OUT_OF_RANGE;
    }
    return result;
}

void LibTec::startTrace()
{
    LibMutex libMutex(s_mutex);
    if (!m_isTraceRunning) {
        LibMutex libMutex(s_traceMutex);
        m_traceCircularBuffer.clear();
        m_isTraceRunning = true;
    }
}

void LibTec::stopTrace()
{
    LibMutex libMutex(s_mutex);
    if (m_isTraceRunning) {
        m_isTraceRunning = false;
    }
}

int LibTec::setTraceResolution(int res)
{
    LibMutex libMutex(s_mutex);
    int result = OKAY;
    switch (res) {
    default:
        result = ERROR_TRACE_RESOLUTION_OUT_OF_RANGE;
        break;
    case TRACE_RES_10:
    case TRACE_RES_100:
        m_traceRes = res;
        break;
    }
    return result;
}

int LibTec::getTraceResolution()
{
    return m_traceRes;
}

int LibTec::getTraceFirstSample()
{
    LibMutex libMutex(s_traceMutex);
    return m_traceCircularBuffer.front();
}

int LibTec::getTraceNumberOfSamples()
{
    LibMutex libMutex(s_traceMutex);
    return m_traceCircularBuffer.size();
}

int LibTec::setTraceNumberOfReadSamples(int number)
{
    LibMutex libMutex(s_traceMutex);
    if (number < 0 || number > m_traceCircularBuffer.size()) {
        return ERROR_TRACE_NUMBER_OF_READ_SAMPLES_OUT_OF_RANGE;
    }
    for (int i = 0; i < number; i++) {
        m_traceCircularBuffer.popFront();
    }
    return OKAY;
}
