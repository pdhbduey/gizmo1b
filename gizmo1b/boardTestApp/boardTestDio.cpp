#include "boardTestDio.h"

BoardTestDio::BoardTestDio()
{
}

BoardTestDio::~BoardTestDio()
{
}

int BoardTestDio::get(uint32 address, uint32& value)
{
    bool isSet;
    switch (address) {
    default:
        return ERROR_ADDR;
    case DIO_IN:
        value = 0;
        m_libDio.getPin(LibDio::DIN_0, isSet);
        value |= isSet ? DIN_0_STATE : 0;
        m_libDio.getPin(LibDio::DIN_1, isSet);
        value |= isSet ? DIN_1_STATE : 0;
        m_libDio.getPin(LibDio::DIN_2, isSet);
        value |= isSet ? DIN_2_STATE : 0;
        m_libDio.getPin(LibDio::DIN_3, isSet);
        value |= isSet ? DIN_3_STATE : 0;
        m_libDio.getPin(LibDio::DIN_4, isSet);
        value |= isSet ? DIN_4_STATE : 0;
        m_libDio.getPin(LibDio::DIN_5, isSet);
        value |= isSet ? DIN_5_STATE : 0;
        break;
    case DIO_OUT:
        value = 0;
        m_libDio.getPin(LibDio::DOUT_0, isSet);
        value |= (isSet ? DOUT_0_ON : DOUT_0_OFF);
        m_libDio.getPin(LibDio::DOUT_1, isSet);
        value |= (isSet ? DOUT_1_ON : DOUT_1_OFF);
        m_libDio.getPin(LibDio::DOUT_2, isSet);
        value |= (isSet ? DOUT_2_ON : DOUT_2_OFF);
        m_libDio.getPin(LibDio::DOUT_3, isSet);
        value |= (isSet ? DOUT_3_ON : DOUT_3_OFF);
        m_libDio.getPin(LibDio::DOUT_4, isSet);
        value |= (isSet ? DOUT_4_ON : DOUT_4_OFF);
        m_libDio.getPin(LibDio::DOUT_5, isSet);
        value |= (isSet ? DOUT_5_ON : DOUT_5_OFF);
        m_libDio.getPin(LibDio::DOUT_6, isSet);
        value |= (isSet ? DOUT_6_ON : DOUT_6_OFF);
        m_libDio.getPin(LibDio::DOUT_7, isSet);
        value |= (isSet ? DOUT_7_ON : DOUT_7_OFF);
        break;
    }
    return OKAY;
}

int BoardTestDio::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case DIO_IN:
        return ERROR_RO;
    case DIO_OUT:
        if (value & DOUT_0_ON) {
            m_libDio.setPin(LibDio::DOUT_0, true);
        }
        if (value & DOUT_0_OFF) {
            m_libDio.setPin(LibDio::DOUT_0, false);
        }
        if (value & DOUT_1_ON) {
            m_libDio.setPin(LibDio::DOUT_1, true);
        }
        if (value & DOUT_1_OFF) {
            m_libDio.setPin(LibDio::DOUT_1, false);
        }
        if (value & DOUT_2_ON) {
            m_libDio.setPin(LibDio::DOUT_2, true);
        }
        if (value & DOUT_2_OFF) {
            m_libDio.setPin(LibDio::DOUT_2, false);
        }
        if (value & DOUT_3_ON) {
            m_libDio.setPin(LibDio::DOUT_3, true);
        }
        if (value & DOUT_3_OFF) {
            m_libDio.setPin(LibDio::DOUT_3, false);
        }
        if (value & DOUT_4_ON) {
            m_libDio.setPin(LibDio::DOUT_4, true);
        }
        if (value & DOUT_4_OFF) {
            m_libDio.setPin(LibDio::DOUT_4, false);
        }
        if (value & DOUT_5_ON) {
            m_libDio.setPin(LibDio::DOUT_5, true);
        }
        if (value & DOUT_5_OFF) {
            m_libDio.setPin(LibDio::DOUT_5, false);
        }
        if (value & DOUT_6_ON) {
            m_libDio.setPin(LibDio::DOUT_6, true);
        }
        if (value & DOUT_6_OFF) {
            m_libDio.setPin(LibDio::DOUT_6, false);
        }
        if (value & DOUT_7_ON) {
            m_libDio.setPin(LibDio::DOUT_7, true);
        }
        if (value & DOUT_7_OFF) {
            m_libDio.setPin(LibDio::DOUT_7, false);
        }
        break;
    }
    return OKAY;
}
