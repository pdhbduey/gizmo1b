#include "libDioOut.h"
#include "het.h"
#include "libWrapHet1.h"

LibDioOut::LibDioOut()
{
    LibWrapGioPort* libWrapHet1 = new LibWrapHet1;
    m_outPinsMap[DOUT_0] = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_12); // 124:HET1_12:DIG_OUT_A0
    m_outPinsMap[DOUT_1] = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_13); //  39:HET1_13:DIG_OUT_A1
    m_outPinsMap[DOUT_2] = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_14); // 125:HET1_14:DIG_OUT_A2
    m_outPinsMap[DOUT_3] = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_15); //  41:HET1_15:DIG_OUT_A3
    m_outPinsMap[DOUT_4] = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_24); //  91:HET1_24:DIG_OUT_B0
    m_outPinsMap[DOUT_5] = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_26); //  92:HET1_26:DIG_OUT_B1
    m_outPinsMap[DOUT_6] = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_28); // 107:HET1_28:DIG_OUT_B2
    m_outPinsMap[DOUT_7] = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_30); // 127:HET1_30:DIG_OUT_B3
}

LibDioOut::~LibDioOut()
{
}

int LibDioOut::setPin(int pin, bool set)
{
    if (m_outPinsMap.find(pin) == m_outPinsMap.end() || !m_outPinsMap[pin]) {
        return INVALID_OUTPUT_PIN;
    }
    m_outPinsMap[pin]->m_libWrapGioPort->setPin(m_outPinsMap[pin]->m_pin, set);
    return OKAY;
}

int LibDioOut::getPin(int pin, bool& isSet)
{
    if (m_outPinsMap.find(pin) == m_outPinsMap.end() || !m_outPinsMap[pin]) {
        return INVALID_OUTPUT_PIN;
    }
    isSet = m_outPinsMap[pin]->m_libWrapGioPort->getPin(m_outPinsMap[pin]->m_pin);
    return OKAY;
}
