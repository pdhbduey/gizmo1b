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
    case TEC_WAVEFORM_SAMPLE_INDEX:
        value = m_customWaveform.size();
        break;
    case TEC_WAVEFORM_SAMPLE_TIME:
        value = m_irefSample.m_time;
        break;
    case TEC_WAVEFORM_SAMPLE_IREF:
        value = *reinterpret_cast<uint32*>(&m_irefSample.m_iref);
        break;
    case TEC_WAVEFORM_CYCLES:
        value = m_customeWaveformCycles;
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
    case TEC_WAVEFORM_SAMPLE_INDEX:
        return ERROR_RO;
    case TEC_CONTROL:
        if (value & DISABLE) {
            m_libTec.enable(false);
            m_libLed.set(LibLed::RED, true);
        }
        if (value & ENABLE) {
            m_libTec.enable(true);
            m_libLed.set(LibLed::RED, false);
        }
        if (value & START_WAVEFORM) {
            uint32 status = LibTec::OKAY;
            if (m_libTec.getWaveformType() == LibTec::WAVEFORM_TYPE_CUSTOM) {
                m_status = m_libTec.setCustomWaveform(m_customWaveform,
                                                       m_customeWaveformCycles);
                status = m_status;
            }
            if (status == LibTec::OKAY) {
                m_libTec.waveformStart();
                m_libLed.set(LibLed::GREEN, false);
            }
        }
        if (value & STOP_WAVEFORM) {
            m_libTec.waveformStop();
            m_libLed.set(LibLed::GREEN, true);
        }
        if (value & CLOSED_LOOP_DISABLE) {
            m_libTec.closedLoopDisable();
        }
        if (value & CLOSED_LOOP_ENABLE) {
            m_libTec.closedLoopEnable();
        }
        if (value & CUSTOM_WAVEFORM_RESET_INDEX) {
            m_customWaveform.clear();
            m_irefSample.clear();
        }
        if (value & CUSTOM_WAVEFORM_INC_INDEX) {
            m_customWaveform.push_back(m_irefSample);
            m_irefSample.clear();
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
    case TEC_WAVEFORM_SAMPLE_TIME:
        m_irefSample.m_time = value;
        break;
    case TEC_WAVEFORM_SAMPLE_IREF:
        m_irefSample.m_iref = *reinterpret_cast<float*>(&value);
        break;
    case TEC_WAVEFORM_CYCLES:
        m_customeWaveformCycles = value;
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
