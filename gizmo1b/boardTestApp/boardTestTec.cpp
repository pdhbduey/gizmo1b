#include <math.h>
#include "libTec.h"
#include "libFault.h"
#include "libDac.h"
#include "LibDelay.h"
#include "boardTestTec.h"

BoardTestTec::BoardTestTec()
{
    m_libTec.start();
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
        value |= m_libTec.isEnabled() ? ENABLE : DISABLE;
        value |= m_libTec.isWaveformStarted() ? START_WAVEFORM : STOP_WAVEFORM;
        value |= m_libTec.isClosedLoopEnabled() ? CLOSED_LOOP_ENABLE : CLOSED_LOOP_DISABLE;
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
    case TEC_DAC_OFFSET:
        {
            float offset = m_libTec.getOffset();
            value = *reinterpret_cast<uint32*>(&offset);
        }
        break;
    case TEC_PROPORTIONAL_GAIN:
        {
            float proportionalGain = m_libTec.getProportionalGain();
            value = *reinterpret_cast<uint32*>(&proportionalGain);
        }
        break;
    case TEC_INTEGRAL_GAIN:
        {
            float integralGain = m_libTec.getIntegrallGain();
            value = *reinterpret_cast<uint32*>(&integralGain);
        }
        break;
    case TEC_DERIVATIVE_GAIN:
        {
            float derivativeGain = m_libTec.getDerivativeGain();
            value = *reinterpret_cast<uint32*>(&derivativeGain);
        }
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
        }
        if (value & ENABLE) {
            m_libTec.enable(true);
        }
        if (value & START_WAVEFORM) {
            m_libTec.waveformStart();
        }
        if (value & STOP_WAVEFORM) {
            m_libTec.waveformStop();
        }
        if (value & CLOSED_LOOP_DISABLE) {
            m_libTec.closedLoopDisable();
        }
        if (value & CLOSED_LOOP_ENABLE) {
            m_libTec.closedLoopEnable();
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
    case TEC_DAC_OFFSET:
        {
            float offset = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.setOffset(offset);
        }
        break;
    case TEC_PROPORTIONAL_GAIN:
        {
            float proportionalGain = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.setProportionalGain(proportionalGain);
        }
        break;
    case TEC_INTEGRAL_GAIN:
        {
            float integralGain = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.setIntegralGain(integralGain);
        }
        break;
    case TEC_DERIVATIVE_GAIN:
        {
            float derivativeGain = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.setDerivativeGain(derivativeGain);
        }
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
