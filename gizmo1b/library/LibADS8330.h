#pragma once

#include <map>
#include <libWrapGioPort.h>
#include <LibAdc.h>
#include <LibMibSpi.h>

class LibADS8330 : public LibAdc
{
public:
    enum Command {
        SEL_AN_IN_CHANNEL_0 = 0b0000 << 12,
        SEL_AN_IN_CHANNEL_1 = 0b0001 << 12,
        WAKE_UP             = 0b1011 << 12,
        READ_CFG_REGISTER   = 0b1100 << 12,
        READ_DATA           = 0b1101 << 12,
        WRITE_CFG_REGISTER  = 0b1110 << 12,
        RESET_CFG_REGISTER  = 0b1111 << 12,
    };
    enum CfgReg {
        MANUAL_CHANNEL_SELECT = 0b0   << 11,
        AUTO_CHANNEL_SELECT   = 0b1   << 11,
        CCLK_HALF_SCLK        = 0b0   << 10,
        CCLK_INTERNAL_25MHz   = 0b1   << 10,
        AUTO_TRIGGER_SOC      = 0b0   <<  9,
        MANUAL_TRIGGER_SOC    = 0b1   <<  9,
        D8_DONT_CARE          = 0b0   <<  8,
        EOC_ACTIVE_HIGH       = 0b0   <<  7,
        EOC_ACTIVE_LOW        = 0b1   <<  7,
        PIN_EOC_IS_INT        = 0b0   <<  6,
        PIN_EOC_IS_EOC        = 0b1   <<  6,
        PIN_EOC_IS_CDI_IN     = 0b0   <<  5,
        PIN_EOC_IS_EOC_OUT    = 0b1   <<  5,
        POWER_DOWN_DISABLED   = 0b111 <<  2,
        TAG_BIT_DISABLED      = 0b0   <<  1,
        TAG_BIT_ENABLED       = 0b1   <<  1,
        SYSTEM_RESET          = 0b0   <<  0,
        NORMAL_OPERATION      = 0b1   <<  0,
    };
    enum Pins {
        SOMI_SW,
        CONVST,
        EOC,
        CS,
    };
public:
    LibADS8330(float vref);
    // LibAdc
    virtual int read(int channel, uint32& value); // 0-65535
    void initialize();
private:
    int write(uint16 txBuffer, uint16& data);
protected:
    LibMibSpi* m_libWrapMibSpi;
    uint32 m_mibSpiGroup;
    uint32 m_somiSelect;
    std::map<int, LibWrapGioPort::Port*> m_pinMap;
    std::map<int, int> m_channelMap;
};
