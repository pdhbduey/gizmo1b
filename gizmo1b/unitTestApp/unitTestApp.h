#ifndef _UNIT_TEST_APP_H_
#define _UNIT_TEST_APP_H_

#include "libTask.h"

class UnitTestApp: public LibTask {
public:
    UnitTestApp(const char* name);
    virtual ~UnitTestApp();
private:
    virtual void run();
};

#endif // _UNIT_TEST_APP_H_
