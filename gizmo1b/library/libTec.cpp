#include "mibspi.h"
#include "libWrapMibSpi5.h"
#include "libMutex.h"
#include "libTec.h"

SemaphoreHandle_t LibTec::s_mutex;
bool LibTec::s_isInitialized;

LibTec::LibTec() :
    m_tecEnable(new LibWrapMibSpi5, PIN_SIMO) // 99:MIBSPI5SIMO[0]:TEC_EN
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

// TEC_ISENSE = [±(15A*0.008ohm*20V/V) + 2.5V] * 6.04K/9.05K
int LibTec::getISense(float& iSense)
{
    LibMutex libMutex(s_mutex);
    float value;
    int result = m_libAdc.read(ISENSE, value);
    if (result != LibAdc::OKAY) {
        return ERROR_ISENSE;
    }

    iSense = (value * 9.05 / 6.04 - 2.5) / 20.0 / 0.008;
    return OKAY;
}

//TEC_VSENSE = (V+ - V-) * 20/21.5 + 2.5V
int LibTec::getVSense(float& vSense)
{
    LibMutex libMutex(s_mutex);
    float value;
    int result = m_libAdc.read(VSENSE, value);
    if (result != LibAdc::OKAY) {
        return ERROR_VSENSE;
    }
    vSense = (value - 2.5) * 21.5 / 20.0;
    return OKAY;
}

void LibTec::setCurrent(float value)
{
}

void LibTec::setCurrent(int loopCount, std::vector<TimeCurrent>& setPoints)
{
}

void LibTec::stopCurrent()
{
}

int LibTec::getLoopValue()
{
    return 0;
}

int LibTec::getCountValue()
{
    return 0;
}
