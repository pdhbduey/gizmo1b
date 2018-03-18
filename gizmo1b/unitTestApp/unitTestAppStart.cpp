#include "FreeRTOS.h"
#include "unitTestAppStart.h"
#include "unitTestApp.h"

void unitTestAppStart()
{
    UnitTestApp unitTestApp("UnitTestApp");
    unitTestApp.start();
    vTaskStartScheduler();
    while (true) {
    }
}
