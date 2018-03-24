#include "unitTestApp.h"
#include "libWrapMibSpi1.h"
#include "boardTestLed.h"
#include "libDac.h"

UnitTestApp::UnitTestApp(const char* name) :
    LibTask(name)
{
}

UnitTestApp::~UnitTestApp()
{
}

void UnitTestApp::run()
{
    LibWrapMibSpi1::test();
    BoardTestLed::test();
    LibDac::test();

    while (true) {
    }
}
