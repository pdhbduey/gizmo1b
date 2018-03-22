#include "FreeRTOS.h"
#include "unitTestAppStart.h"
#include "unitTestApp.h"
#include "libRaisePrivilege.h"

void unitTestAppStart()
{
    LibRaisePrivilege libRaisePrivilege;
    UnitTestApp unitTestApp("UnitTestApp");
    unitTestApp.start();
    vTaskStartScheduler();
    while (true) {
    }
}
