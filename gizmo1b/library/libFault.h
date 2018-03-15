#ifndef _LIB_FAULT_H_
#define _LIB_FAULT_H_

#include "libCommon.h"

class LibFault
{
public:
    enum State {
        TEC_OCD_POS,
        TEC_OCD_NEG,
        OVERTEMP1, // only valid if NTC1 present
        OVERTEMP2, // only valid if NTC2 present
        STATE_MAX
    };
    enum Ntc {
        NTC1,
        NTC2,
        NTC_MAX
    };
    enum Reset {
        RESET_FAULT,
        RESET_FAULT_MAX
    };
    enum Status {
        OKAY,
        INVALID_STATE,
        INVALID_NTC,
    };
public:
    LibFault();
    virtual ~LibFault();
    void reset();
    int readState(int state, bool& isFault); // monitor every 1s
    int readNtcPresent(int ntc, bool& isNtcPresent);
private:
    static bool s_isInitialized;
    static struct LibCommon::Port s_statePort[STATE_MAX];
    static struct LibCommon::Port s_ntcPort[NTC_MAX];
    static struct LibCommon::Port s_resetPort[RESET_FAULT_MAX];
};

#endif // _LIB_FAULT_H_
