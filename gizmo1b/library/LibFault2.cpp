#include <LibFault2.h>
#include <libWrapHet1.h>
#include <het.h>

LibFault2::LibFault2()
{
    m_drvErrClr             = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_20); // 141:N2HET1[20]:DRV_ERR_CLR
    m_ntcMap[NTC1]          = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_22); //  15:N2HET1[22]:NTC_PRESENT1
    m_ntcMap[NTC2]          = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_24); //  91:N2HET1[24]:NTC_PRESENT2
    m_faultMap[TEC_OCD_POS] = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_3);  //  24:N2HET1 [3]:TEC_OCD_POS
    m_faultMap[TEC_OCD_NEG] = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_6);  //  38:N2HET1 [6]:TEC_OCD_NEG
    m_faultMap[OVERTEMP1]   = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_7);  //  33:N2HET1 [7]:OVERTEMP1
    m_faultMap[OVERTEMP2]   = new LibWrapGioPort::Port(new LibWrapHet1, PIN_HET_8);  // 106:N2HET1 [8]:OVERTEMP2
    hetREG1->DIR |=   (uint32)1U << PIN_HET_20;
    hetREG1->DIR &= ~((uint32)1U << PIN_HET_22
                 |    (uint32)1U << PIN_HET_24
                 |    (uint32)1U << PIN_HET_3
                 |    (uint32)1U << PIN_HET_6
                 |    (uint32)1U << PIN_HET_7
                 |    (uint32)1U << PIN_HET_8);
}
