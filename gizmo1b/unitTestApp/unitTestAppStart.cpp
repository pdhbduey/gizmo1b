#include "FreeRTOS.h"
#include "os_task.h"
#include "libRaisePrivilege.h"
#include "unitTestApp.h"
#include "unitTestAppStart.h"

static void launcherTask(void* parameter)
{
    LibRaisePrivilege libRaisePrivilege;
    UnitTestApp unitTestApp("UnitTestApp");
    unitTestApp.start();
    while (true) {
    }
}

// Interrupts are enabled in the scheduler so application must be started
// AFTER the scheduler because we need interrupts enabled when we run the
// application
void unitTestAppStart()
{
    TaskHandle_t taskHandle;
    xTaskCreate(launcherTask, "Launcher", configMINIMAL_STACK_SIZE, NULL,
                                             tskIDLE_PRIORITY + 1, &taskHandle);
    vTaskStartScheduler();
    while (true) {
    }
}
