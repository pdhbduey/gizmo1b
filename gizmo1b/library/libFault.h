#ifndef _LIB_FAULT_H_
#define _LIB_FAULT_H_

#include <map>
#include "libWrapGioPort.h"

class LibFault
{
public:
    enum Fault {
        TEC_OCD_POS,
        TEC_OCD_NEG,
        OVERTEMP1, // valid if NTC1 present
        OVERTEMP2, // valid if NTC2 present
    };
    enum Ntc {
        NTC1,
        NTC2,
    };
    enum Reset {
        RESET_FAULT,
    };
    enum Status {
        OKAY,
        ERROR_INVALID_FAULT,
        ERROR_INVALID_NTC,
        ERROR_NOT_IMPLEMENTED,
    };
public:
    LibFault();
    virtual ~LibFault();
    void reset();
    int getFault(int fault, bool& isFault);
    int getNtcPresent(int ntc, bool& isNtcPresent);
private:
    std::map<int, LibWrapGioPort::Port*> m_faultMap;
    std::map<int, LibWrapGioPort::Port*> m_ntcMap;
    LibWrapGioPort::Port m_drvErrClr;
};

#endif // _LIB_FAULT_H_
