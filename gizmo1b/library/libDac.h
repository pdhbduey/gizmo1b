#ifndef _LIB_DAC_H_
#define _LIB_DAC_H_

#include <map>
#include "libWrapMibSpi1.h"
#include "libWrapGioPort.h"

class LibDac
{
public:
    enum Status {
        OKAY,
        ERROR_SET_VALUE_OUT_OF_RANGE,
        ERROR_TIME_OUT,
    };
public:
    LibDac();
    virtual ~LibDac();
    int set(float value); // 0-5V
    static void test();
private:
    enum DacCtrl {
        CLR,
        SYNC,
        LDAC,
    };
    enum Cmd {
        CMD_WR_REG_OR_GAINS                = 0, // Write to input register n or to gains
        CMD_UPDATE_ONE_DAC                 = 1, // Software LDAC, update DAC register n
        CMD_WR_ONE_REG_AND_UPDATE_ALL_DACS = 2, // Write to input register n and update all DAC registers
        CMD_WR_ONE_REG_AND_UPDATE_ONE_DAC  = 3, // Write to input register n and update DAC register n
        CMD_POWER_DAC                      = 4, // Set DAC power up or -down mode
        CMD_RESET                          = 5, // Software reset
        CMD_SET_LDAC_PIN                   = 6, // Set LDAC registers
        CMD_ENABLE_INT_REF                 = 7, // Enable or disable the internal reference
    };
    enum CmdShift {
        CMD_SHIFT = 3,
    };
    enum Addr {
        ADDR_DAC_A       = 0, // DAC-A input register
        ADDR_DAC_B       = 1, // DAC-B input register
        ADDR_DAC_GAIN    = 2,
        ADDR_DAC_A_AND_B = 7, // DAC-A and DAC-B input registers
    };
    enum AddrShift {
        ADDR_SHIFT = 0,
    };
    enum Gain {
        GAIN_DAC_B_2_DAC_A_2 = 0, // Gain: DAC-B gain = 2, DAC-A gain = 2 (default with internal VREF)
        GAIN_DAC_B_2_DAC_A_1 = 1, // Gain: DAC-B gain = 2, DAC-A gain = 1
        GAIN_DAC_B_1_DAC_A_2 = 2, // Gain: DAC-B gain = 1, DAC-A gain = 2
        GAIN_DAC_B_1_DAC_A_1 = 3, // Gain: DAC-B gain = 1, DAC-A gain = 1 (power-on default)
    };
    enum Power {
        POWER_UP_DAC_A                            =  1, // Power up DAC-A
        POWER_UP_DAC_B                            =  2, // Power up DAC-B
        POWER_UP_DAC_A_AND_DAC_B                  =  3, // Power up DAC-A and DAC-B
        POWER_DOWN_DAC_A_1KOHM_TO_GND             =  9, // Power down DAC-A; 1 kΩ to GND
        POWER_DOWN_DAC_B_1KOHM_TO_GND             = 10, // Power down DAC-B; 1 kΩ to GND
        POWER_DOWN_DAC_A_AND_DAC_B_1KOHM_TO_GND   = 11, // Power down DAC-A and DAC-B; 1 kΩ to GND
        POWER_DOWN_DAC_A_100KOHM_TO_GND           = 17, // Power down DAC-A; 100 kΩ to GND
        POWER_DOWN_DAC_B_100KOHM_TO_GND           = 18, // Power down DAC-B; 100 kΩ to GND
        POWER_DOWN_DAC_A_AND_DAC_B_100KOHM_TO_GND = 19, // Power down DAC-A and DAC-B; 100 kΩ to GND
        POWER_DOWN_DAC_A_HI_Z                     = 25, // Power down DAC-A; Hi-Z
        POWER_DOWN_DAC_B_HI_Z                     = 26, // Power down DAC-B; Hi-Z
        POWER_DOWN_DAC_A_AND_DAC_B_HI_Z           = 27, // Power down DAC-A and DAC-B; Hi-Z
    };
    enum Reset {
        RESET_AND_UPDATE = 0, // Reset DAC-A and DAC-B input register and update all DACs
        POWER_ON_RESET   = 1, // Reset all registers and update all DACs (Power-on-reset update)
    };
    enum SetLdacPin {
        SET_LDAC_PIN_ACTIVE_DAC_B_AND_DAC_A        = 0, // LDAC pin active for DAC-B and DAC-A
        SET_LDAC_PIN_ACTIVE_DAC_B_INACTIVE_DAC_A   = 1, // LDAC pin active for DAC-B; inactive for DAC-A
        SET_LDAC_PIN_INACTIVE_DAC_B_ACTIVE_DAC_A   = 2, // LDAC pin inactive for DAC-B; active for DAC-A
        SET_LDAC_PIN_INACTIVE_DAC_B_INACTIVE_DAC_A = 3, // LDAC pin inactive for DAC-B and DAC-A
    };
    enum EnableIntRef {
        DISABLE_INT_REF_AND_RESET_DAC_GAINS_TO_1 = 0, // Disable internal reference and reset DACs to gain = 1
        ENABLE_INT_REF_AND_RESET_DAC_GAINS_TO_2  = 1, // Enable internal reference and reset DACs to gain = 2
    };
private:
    int writeDac(uint16* txBuffer);
private:
    LibWrapMibSpi1 m_libWrapMibSpi1;
    std::map<int, LibWrapGioPort::Port*> m_dacCtrlMap;
};

#endif // _LIB_DAC_H_
