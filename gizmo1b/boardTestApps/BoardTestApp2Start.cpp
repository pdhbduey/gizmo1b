#include <FreeRTOS.h>
#include <os_task.h>
#include <libRaisePrivilege.h>
#include <BoardTestApp2.h>
#include <BoardTestApp2Start.h>

static void launcherTask(void* parameter)
{
    LibRaisePrivilege libRaisePrivilege;
    BoardTestApp2 boardTestApp2("BoardTestApp2");
    boardTestApp2.start();
    while (true) {
    }
}

// Interrupts are enabled in the scheduler so application must be started
// AFTER the scheduler because we need interrupts enabled when we run the
// application
void boardTestApp2Start()
{
    TaskHandle_t taskHandle;
    xTaskCreate(launcherTask, "Launcher", configMINIMAL_STACK_SIZE, NULL,
                                             tskIDLE_PRIORITY + 1, &taskHandle);
    vTaskStartScheduler();
    while (true) {
    }
}
