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
    m_libTec.setRefCurrent(0);
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
        value |= m_isWaveformStarted ? START_WAVEFORM : STOP_WAVEFORM;
        break;
    case TEC_IREF_VALUE:
        {
            float refCurrentValue = m_libTec.getRefCurrent();
            value = *reinterpret_cast<uint32*>(&refCurrentValue);
        }
        break;
    case TEC_ISENSE_VALUE:
        {
            float iSense;
            m_status = m_libTec.getISense(iSense);
            if (m_status == LibTec::OKAY) {
                value = *reinterpret_cast<uint32*>(&iSense);
            }
        }
        break;
    case TEC_VSENSE_VALUE:
        {
            float vSense;
            m_status = m_libTec.getVSense(vSense);
            if (m_status == LibTec::OKAY) {
                value = *reinterpret_cast<uint32*>(&vSense);
            }
        }
        break;
    case TEC_STATUS:
        value = m_status;
        break;
    case TEC_WAVEFORM_TYPE:
        value = m_libTec.getWaveformType();
        break;
    case TEC_WAVEFORM_PERIOD:
        value = m_libTec.getWaveformPeriod();
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
    case TEC_STATUS:
        return ERROR_RO;
    case TEC_CONTROL:
        if (value & DISABLE) {
            m_libTec.enable(false);
            m_isEnabled = false;
        }
        if (value & ENABLE) {
            m_libTec.enable(true);
            m_isEnabled = true;
        }
        if (value & START_WAVEFORM) {
            m_libTec.waveformStart();
            m_isWaveformStarted = true;
        }
        if (value & STOP_WAVEFORM) {
            m_libTec.waveformStop();
            m_isWaveformStarted = false;
        }
        break;
    case TEC_IREF_VALUE:
        {
            float refCurrentValue = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.setRefCurrent(refCurrentValue);
        }
        break;
    case TEC_WAVEFORM_TYPE:
        m_status = m_libTec.setWaveformType(value);
        break;
    case TEC_WAVEFORM_PERIOD:
        m_status = m_libTec.setWaveformPeriod(value);
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
