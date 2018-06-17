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
        // large ranges:
        if (address >= SNAPSHOT_SAMPLES_VSENSE
         && address <= SNAPSHOT_SAMPLES_VSENSE_MAX) {
            int sample = address - SNAPSHOT_SAMPLES_VSENSE;
            float f;
            m_status = m_libTec.getSnapshotVsense(sample, f);
            value = *reinterpret_cast<uint32*>(&f);
            break;
        }
        else if (address >= SNAPSHOT_SAMPLES_ISENSE
              && address <= SNAPSHOT_SAMPLES_ISENSE_MAX) {
            int sample = address - SNAPSHOT_SAMPLES_ISENSE;
            float f;
            m_status = m_libTec.getSnapshotIsense(sample, f);
            value = *reinterpret_cast<uint32*>(&f);
            break;
        }
        else if (address >= SNAPSHOT_SAMPLES_IREF
              && address <= SNAPSHOT_SAMPLES_IREF_MAX) {
            int sample = address - SNAPSHOT_SAMPLES_IREF;
            float f;
            m_status = m_libTec.getSnapshotIref(sample, f);
            value = *reinterpret_cast<uint32*>(&f);
            break;
        }
        else if (address >= SNAPSHOT_SAMPLES_TEMPERATURE1
              && address <= SNAPSHOT_SAMPLES_TEMPERATURE1_MAX) {
            int sample = address - SNAPSHOT_SAMPLES_TEMPERATURE1;
            float f;
            m_status = m_libTec.getSnapshotT1(sample, f);
            value = *reinterpret_cast<uint32*>(&f);
            break;
        }
        else if (address >= SNAPSHOT_SAMPLES_TEMPERATURE2
              && address <= SNAPSHOT_SAMPLES_TEMPERATURE2_MAX) {
            int sample = address - SNAPSHOT_SAMPLES_TEMPERATURE2;
            float f;
            m_status = m_libTec.getSnapshotT2(sample, f);
            value = *reinterpret_cast<uint32*>(&f);
            break;
        }
        else if (address >= SNAPSHOT_SAMPLES_TEMPERATURE3
              && address <= SNAPSHOT_SAMPLES_TEMPERATURE3_MAX) {
            int sample = address - SNAPSHOT_SAMPLES_TEMPERATURE3;
            float f;
            m_status = m_libTec.getSnapshotT3(sample, f);
            value = *reinterpret_cast<uint32*>(&f);
            break;
        }
        else if (address >= SNAPSHOT_SAMPLES_TEMPERATURE4
              && address <= SNAPSHOT_SAMPLES_TEMPERATURE4_MAX) {
            int sample = address - SNAPSHOT_SAMPLES_TEMPERATURE4;
            float f;
            m_status = m_libTec.getSnapshotT4(sample, f);
            value = *reinterpret_cast<uint32*>(&f);
            break;
        }
        // errors
        else {
            return ERROR_ADDR;
        }
    case SNAPSHOT_CONTROL:
    case TRACE_CONTROL:
    case TRACE_NUMBER_OF_READ_SAMPLES:
        return ERROR_WO;
    case TEC_CONTROL:
        value = 0;
        value |= (m_libTec.isEnabled()           ? ENABLE             : DISABLE);
        value |= (m_libTec.isWaveformStarted()   ? START_WAVEFORM     : STOP_WAVEFORM);
        value |= (m_libTec.isClosedLoopEnabled() ? CLOSED_LOOP_ENABLE : CLOSED_LOOP_DISABLE);
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
        value = *reinterpret_cast<uint32*>(&m_irefSample.m_value);
        break;
    case TEC_WAVEFORM_CYCLES:
        value = m_customeWaveformCycles;
        break;
    case TEC_VOUT_MAX:
        {
            float voutLimit = m_libTec.getVoutMax();
            value = *reinterpret_cast<uint32*>(&voutLimit);
        }
        break;
    case SNAPSHOT_STATUS:
        if (m_libTec.isSnapshotRunning()) {
            m_snapshotStatus |=  LibTec::SNAPSHOT_IN_PROGRESS;
        }
        else {
            m_snapshotStatus &= ~LibTec::SNAPSHOT_IN_PROGRESS;
        }
        value = m_snapshotStatus;
        break;
    case SNAPSHOT_RESOLUTION:
        value = m_libTec.getSnapshotResolution();
        break;
    case SNAPSHOT_NUMBER_OF_SAMPLES:
        value = m_libTec.getSnapshotNumberOfSamples();
        break;
    case TRACE_RESOLUTION:
        value = m_libTec.getTraceNumberOfSamples();
        break;
    case TRACE_FIRST_SAMPLE:
        value = m_libTec.getTraceFirstSample();
        break;
    case TRACE_NUMBER_OF_SAMPLES:
        value = m_libTec.getTraceNumberOfSamples();
        break;
    case TRACE_STATUS:
        value = m_traceStatus;
        break;
    case HEATER_CONTROL:
        value = 0;
        value |= (m_libTec.isHeaterEnabled()           ? HEATER_ENABLE             : HEATER_DISABLE);
        value |= (m_libTec.isHeaterClosedLoopEnabled() ? HEATER_CLOSED_LOOP_ENABLE : HEATER_CLOSED_LOOP_DISABLE);
        value |= (m_libTec.isHeaterWaveformStarted()   ? HEATER_START_WAVEFORM     : HEATER_STOP_WAVEFORM);
        break;
    case HEATER_STATUS:
        value = m_status;
        break;
    case HEATER_TREF_VALUE:
        {
            float f = m_libTec.getHeaterRefTemperature();
            value = *reinterpret_cast<uint32*>(&f);
        }
        break;
    case HEATER_IMAX_VALUE:
        {
            float f = m_libTec.getHeaterImax();
            value = *reinterpret_cast<uint32*>(&f);
        }
        break;
    case HEATER_TIN_SELECT:
        value = m_libTec.heaterGetTin();
        break;
    case HEATER_PROPORTIONAL_GAIN:
        {
            float gain = m_libTec.heaterGetProportionalGain();
            value = *reinterpret_cast<uint32*>(&gain);
        }
        break;
    case HEATER_INTEGRAL_GAIN:
        {
            float gain = m_libTec.heaterGetIntegrallGain();
            value = *reinterpret_cast<uint32*>(&gain);
        }
        break;
    case HEATER_DERIVATIVE_GAIN:
        {
            float gain = m_libTec.heaterGetDerivativeGain();
            value = *reinterpret_cast<uint32*>(&gain);
        }
        break;
    case HEATER_WAVEFORM_SAMPLE_INDEX:
        value = m_heaterCustomWaveform.size();
        break;
    case HEATER_WAVEFORM_SAMPLE_TIME:
        value = m_heaterTrefSample.m_time;
        break;
    case HEATER_WAVEFORM_SAMPLE_TREF:
        value = *reinterpret_cast<uint32*>(&m_heaterTrefSample.m_value);
        break;
    case HEATER_WAVEFORM_CYCLES:
        value = m_heaterCustomeWaveformCycles;
        break;
    }
    return OKAY;
}

int BoardTestTec::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        // large ranges and errors
        if (address >= SNAPSHOT_SAMPLES_VSENSE
         && address <= SNAPSHOT_SAMPLES_VSENSE_MAX) {
            return ERROR_RO;
        }
        else if (address >= SNAPSHOT_SAMPLES_ISENSE
              && address <= SNAPSHOT_SAMPLES_ISENSE_MAX) {
            return ERROR_RO;
        }
        else if (address >= SNAPSHOT_SAMPLES_IREF
              && address <= SNAPSHOT_SAMPLES_IREF_MAX) {
            return ERROR_RO;
        }
        else if (address >= SNAPSHOT_SAMPLES_TEMPERATURE1
              && address <= SNAPSHOT_SAMPLES_TEMPERATURE1_MAX) {
            return ERROR_RO;
        }
        else if (address >= SNAPSHOT_SAMPLES_TEMPERATURE2
              && address <= SNAPSHOT_SAMPLES_TEMPERATURE2_MAX) {
            return ERROR_RO;
        }
        else if (address >= SNAPSHOT_SAMPLES_TEMPERATURE3
              && address <= SNAPSHOT_SAMPLES_TEMPERATURE3_MAX) {
            return ERROR_RO;
        }
        else if (address >= SNAPSHOT_SAMPLES_TEMPERATURE4
              && address <= SNAPSHOT_SAMPLES_TEMPERATURE4_MAX) {
            return ERROR_RO;
        }
        else {
            return ERROR_ADDR;
        }
    case TEC_ISENSE_VALUE:
    case TEC_VSENSE_VALUE:
    case TEC_STATUS:
    case TEC_WAVEFORM_SAMPLE_INDEX:
    case SNAPSHOT_STATUS:
    case TRACE_STATUS:
    case TRACE_FIRST_SAMPLE:
    case TRACE_NUMBER_OF_SAMPLES:
    case HEATER_STATUS:
    case HEATER_WAVEFORM_SAMPLE_INDEX:
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
        m_irefSample.m_value = *reinterpret_cast<float*>(&value);
        break;
    case TEC_WAVEFORM_CYCLES:
        m_customeWaveformCycles = value;
        break;
    case TEC_VOUT_MAX:
        {
            float voutLimit = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.setVoutMax(voutLimit);
        }
        break;
    case SNAPSHOT_CONTROL:
        if (value & SNAPSHOT_START) {
            m_libTec.startSnaphot();
        }
        if (value & SNAPSHOT_STOP) {
            m_libTec.stopSnapshot();
        }
        break;
    case SNAPSHOT_RESOLUTION:
        m_snapshotStatus = m_libTec.setSnapshotResolution(value);
        break;
    case SNAPSHOT_NUMBER_OF_SAMPLES:
        m_snapshotStatus = m_libTec.setSnapshotNumberOfSamples(value);
        break;
    case TRACE_RESOLUTION:
        m_traceStatus = m_libTec.setTraceResolution(value);
        break;
    case TRACE_CONTROL:
        if (value & TRACE_START) {
            m_libTec.startTrace();
        }
        if (value & TRACE_STOP) {
            m_libTec.stopTrace();
        }
        break;
    case TRACE_NUMBER_OF_READ_SAMPLES:
        m_traceStatus = m_libTec.setTraceNumberOfReadSamples(value);
        break;
    case HEATER_CONTROL:
        if (value & HEATER_DISABLE) {
            m_libTec.heaterEnable(false);
        }
        if (value & HEATER_ENABLE) {
            m_libTec.heaterEnable(true);
        }
        if (value & HEATER_START_WAVEFORM) {
            m_status = m_libTec.heaterSetCustomWaveform(m_heaterCustomWaveform,
                                                        m_heaterCustomeWaveformCycles);
            if (m_status == LibTec::HEATER_OKAY) {
                m_libTec.heaterWaveformStart();
            }
        }
        if (value & HEATER_STOP_WAVEFORM) {
           m_libTec.heaterWaveformStop();
        }
        if (value & HEATER_CLOSED_LOOP_DISABLE) {
            m_libTec.heaterClosedLoopDisable();
        }
        if (value & HEATER_CLOSED_LOOP_ENABLE) {
            m_libTec.heaterClosedLoopEnable();
        }
        if (value & HEATER_CUSTOM_WAVEFORM_RESET_INDEX) {
            m_heaterCustomWaveform.clear();
            m_heaterTrefSample.clear();
        }
        if (value & HEATER_CUSTOM_WAVEFORM_INC_INDEX) {
            m_heaterCustomWaveform.push_back(m_heaterTrefSample);
            m_heaterTrefSample.clear();
        }
        break;
    case HEATER_TREF_VALUE:
        {
            float f = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.setHeaterRefTemperature(f);
        }
        break;
    case HEATER_IMAX_VALUE:
        {
            float f = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.setHeaterImax(f);
        }
        break;
    case HEATER_TIN_SELECT:
        m_status = m_libTec.heaterSetTin(value);
        break;
    case HEATER_PROPORTIONAL_GAIN:
        {
            float gain = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.heaterSetProportionalGain(gain);
        }
        break;
    case HEATER_INTEGRAL_GAIN:
        {
            float gain = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.heaterSetIntegralGain(gain);
        }
        break;
    case HEATER_DERIVATIVE_GAIN:
        {
            float gain = *reinterpret_cast<float*>(&value);
            m_status = m_libTec.heaterSetDerivativeGain(gain);
        }
        break;
    case HEATER_WAVEFORM_SAMPLE_TIME:
        m_heaterTrefSample.m_time = value;
        break;
    case HEATER_WAVEFORM_SAMPLE_TREF:
        m_heaterTrefSample.m_value = *reinterpret_cast<float*>(&value);
        break;
    case HEATER_WAVEFORM_CYCLES:
        m_heaterCustomeWaveformCycles = value;
        break;
    }
    return OKAY;
}
