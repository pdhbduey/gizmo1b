#include "unitTestApp.h"
#include "libWrapMibSpi1.h"
#include "boardTestLed.h"
#include "libDac.h"
#include "BoardTestTec.h"

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
    BoardTestTec::test();

    while (true) {
    }
}
