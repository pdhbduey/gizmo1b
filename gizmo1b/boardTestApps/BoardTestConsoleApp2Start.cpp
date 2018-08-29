#include <FreeRTOS.h>
#include <os_task.h>
#include <libRaisePrivilege.h>
#include <BoardTestConsoleApp2.h>
#include <BoardTestConsoleApp2Start.h>

static void launcherTask(void* parameter)
{
    LibRaisePrivilege libRaisePrivilege;
    BoardTestConsoleApp2 boardTestConsoleApp2("BoardTestConsoleApp2");
    boardTestConsoleApp2.start();
    while (true) {
    }
}

// Interrupts are enabled in the scheduler so application must be started
// AFTER the scheduler because we need interrupts enabled when we run the
// application
void boardTestConsoleApp2Start()
{
    TaskHandle_t taskHandle;
    xTaskCreate(launcherTask, "Launcher", configMINIMAL_STACK_SIZE, NULL,
                                             tskIDLE_PRIORITY + 1, &taskHandle);
    vTaskStartScheduler();
    while (true) {
    }
}
