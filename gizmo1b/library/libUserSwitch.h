#ifndef _LIB_USER_SWITCH_H_
#define _LIB_USER_SWITCH_H_

#include <map>
#include "libWrapGioPort.h"

class LibUserSwitch
{
public:
    enum Control {
        USER_SWITCH_A,
        USER_SWITCH_B,
    };
    enum Status {
        OKAY,
        INVALID_PIN,
    };
public:
    LibUserSwitch();
    virtual ~LibUserSwitch();
    int get(int sw, bool& isSet);
private:
    bool isSwitchValid(int sw);
private:
    std::map<int, LibWrapGioPort::Port*> m_userSwitchMap;
};

#endif // _LIB_USER_SWITCH_H_
