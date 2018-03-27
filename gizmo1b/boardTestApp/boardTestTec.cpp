#include <math.h>
#include "libTec.h"
#include "libFault.h"
#include "libDac.h"
#include "LibDelay.h"
#include "boardTestTec.h"

BoardTestTec::BoardTestTec() :
    m_isEnabled(false)
{
    m_libTec.enable(false);
}

BoardTestTec::~BoardTestTec()
{
}

int BoardTestTec::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case TEC_CONTROL:
        value = 0;
        value |= m_isEnabled ? ENABLE : DISABLE;
        break;
    case TEC_TIME_VALUE:
        value = m_timeValue;
        break;
    case TEC_CURRENT_VALUE:
        value = *reinterpret_cast<uint32*>(&m_currentValue);
        break;
    case TEC_CURRENT_COUNT:
        value = m_setPoints.size();
        break;
    case TEC_CURRENT_LOOP_COUNT:
        value = m_loopCount;
        break;
    case TEC_ISENSE_VALUE:
        {
            float iSense = m_libTec.getISense();
            value = *reinterpret_cast<uint32*>(&iSense);
        }
        break;
    case TEC_VSENSE_VALUE:
        {
            float vSense = m_libTec.getVSense();
            value = *reinterpret_cast<uint32*>(&vSense);
        }
        break;
    case TEC_LOOP_VALUE:
        value = m_libTec.getLoopValue();
        break;
    case TEC_COUNT_VALUE:
        value = m_libTec.getCountValue();
        break;
    }
    return OKAY;
}

int BoardTestTec::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case TEC_ISENSE_VALUE:
    case TEC_VSENSE_VALUE:
    case TEC_CURRENT_COUNT:
    case TEC_LOOP_VALUE:
    case TEC_COUNT_VALUE:
        return ERROR_RO;
    case TEC_CONTROL:
        if (value & DISABLE) {
            m_libTec.enable(false);
        }
        if (value & ENABLE) {
            m_libTec.enable(true);
        }
        if (value & RESET_CURRENT_COUNTER) {
            m_setPoints.clear();
        }
        if (value & START_CURRENT_WAVEFORM) {
            if (m_setPoints.size() > 1) {
                m_libTec.setCurrent(m_loopCount, m_setPoints);
            }
            else if (m_setPoints.size() == 1) {
                m_libTec.setCurrent(m_currentValue);
            }
        }
        if (value & STOP_CURRENT_WAVEFORM) {
            m_libTec.stopCurrent();
        }
        break;
    case TEC_TIME_VALUE:
        m_timeValue = value;
        break;
    case TEC_CURRENT_VALUE:
        {
            m_currentValue = *reinterpret_cast<float*>(&value);
            LibTec::TimeCurrent timeCurrent(m_timeValue, m_currentValue);
            m_setPoints.push_back(timeCurrent);
        }
        break;
    case TEC_CURRENT_LOOP_COUNT:
        m_loopCount = value;
        break;
    }
    return OKAY;
}

void BoardTestTec::test()
{
    LibTec libTec;
    LibFault libFault;
    LibDac libDac;
    libTec.enable(false);
    libFault.reset();
    libDac.set(2.5);
    libTec.enable(true);
    while (true) {
        // Square wave
//        libDac.set(5.0);
//        LibDelay::us(1000);
//        libDac.set(2.5);
//        LibDelay::us(1000);
        // Sine wave
        for (int i = 0; i < 360; i++) {
            float v = 2.5 + 1.5 * sin(i * 2 * 3.141 / 360);
            libDac.set(v);
            LibDelay::us(5400);
        }
    }
}
