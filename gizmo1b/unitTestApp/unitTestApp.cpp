#include "unitTestApp.h"
#include "libWrapMibSpi1.h"

UnitTestApp::UnitTestApp(const char* name) :
    LibTask(name)
{
}

UnitTestApp::~UnitTestApp()
{
}

void UnitTestApp::run()
{
    bool isPass = LibWrapMibSpi1::test();
    ASSERT(isPass);

    while (true) {
    }
}
