#include "libDio.h"
#include "het.h"
#include "libWrapHet1.h"

LibDio::LibDio()
{
    LibWrapGioPort* libWrapGioPort = new LibWrapHet1;
    m_inMap[DIN_0]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_6);  //  38:HET1_06:DIG_IN_A0
    m_inMap[DIN_1]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_7);  //  33:HET1_07:DIG_IN_A1
    m_inMap[DIN_2]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_8);  // 106:HET1_08:DIG_IN_A2
    m_inMap[DIN_3]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_9);  //  35:HET1_09:DIG_IN_A3
    m_inMap[DIN_4]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_10); // 118:HET1_10:DIG_IN_A4
    m_inMap[DIN_5]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_11); //   6:HET1_11:DIG_IN_A5
    m_inMap[DIN_6]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_16); // 139:HET1_16:DIG_IN_B0
    m_inMap[DIN_7]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_18); // 140:HET1_18:DIG_IN_B1
    m_inMap[DIN_8]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_20); // 141:HET1_20:DIG_IN_B2
    m_inMap[DIN_9]   = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_22); //  15:HET1_22:DIG_IN_B3
    m_outMap[DOUT_0] = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_12); // 124:HET1_12:DIG_OUT_A0
    m_outMap[DOUT_1] = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_13); //  39:HET1_13:DIG_OUT_A1
    m_outMap[DOUT_2] = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_14); // 125:HET1_14:DIG_OUT_A2
    m_outMap[DOUT_3] = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_15); //  41:HET1_15:DIG_OUT_A3
    m_outMap[DOUT_4] = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_24); //  91:HET1_24:DIG_OUT_B0
    m_outMap[DOUT_5] = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_26); //  92:HET1_26:DIG_OUT_B1
    m_outMap[DOUT_6] = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_28); // 107:HET1_28:DIG_OUT_B2
    m_outMap[DOUT_7] = new LibWrapGioPort::Port(libWrapGioPort, PIN_HET_30); // 127:HET1_30:DIG_OUT_B3
}

LibDio::~LibDio()
{
}

int LibDio::setPin(int pin, bool set)
{
    if (m_outMap.find(pin) == m_outMap.end() || !m_outMap[pin]) {
        return INVALID_OUTPUT_PIN;
    }
    m_outMap[pin]->m_libWrapGioPort->setPin(m_outMap[pin]->m_pin, set);
    return OKAY;
}

int LibDio::getPin(int pin, bool& isSet)
{
    if (m_inMap.find(pin) == m_inMap.end() || !m_inMap[pin]) {
        return INVALID_INPUT_PIN;
    }
    isSet = m_inMap[pin]->m_libWrapGioPort->getPin(m_inMap[pin]->m_pin);
    return OKAY;
}
