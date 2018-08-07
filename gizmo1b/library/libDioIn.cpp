#include "libDioIn.h"
#include "het.h"
#include "libWrapHet1.h"

LibDioIn::LibDioIn()
{
    LibWrapGioPort* libWrapHet1 = new LibWrapHet1;
    m_inPinsMap[DIN_0]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_6);  //  38:HET1_06:DIG_IN_A0
    m_inPinsMap[DIN_1]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_7);  //  33:HET1_07:DIG_IN_A1
    m_inPinsMap[DIN_2]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_8);  // 106:HET1_08:DIG_IN_A2
    m_inPinsMap[DIN_3]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_9);  //  35:HET1_09:DIG_IN_A3
    m_inPinsMap[DIN_4]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_10); // 118:HET1_10:DIG_IN_A4
    m_inPinsMap[DIN_5]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_11); //   6:HET1_11:DIG_IN_A5
    m_inPinsMap[DIN_6]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_16); // 139:HET1_16:DIG_IN_B0
    m_inPinsMap[DIN_7]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_18); // 140:HET1_18:DIG_IN_B1
    m_inPinsMap[DIN_8]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_20); // 141:HET1_20:DIG_IN_B2
    m_inPinsMap[DIN_9]   = new LibWrapGioPort::Port(libWrapHet1, PIN_HET_22); //  15:HET1_22:DIG_IN_B3
}

LibDioIn::~LibDioIn()
{
}

int LibDioIn::getPin(int pin, bool& isSet)
{
    if (m_inPinsMap.find(pin) == m_inPinsMap.end() || !m_inPinsMap[pin]) {
        return INVALID_INPUT_PIN;
    }
    isSet = m_inPinsMap[pin]->m_libWrapGioPort->getPin(m_inPinsMap[pin]->m_pin);
    return OKAY;
}
