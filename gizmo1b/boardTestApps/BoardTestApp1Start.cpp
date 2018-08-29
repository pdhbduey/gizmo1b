#include <FreeRTOS.h>
#include <os_task.h>
#include <libRaisePrivilege.h>
#include <boardTestApp1.h>
#include <BoardTestApp1Start.h>


static void launcherTask(void* parameter)
{
    LibRaisePrivilege libRaisePrivilege;
    BoardTestApp1 boardTestApp1("BoardTestApp1");
    boardTestApp1.start();
    while (true) {
    }
}

// Interrupts are enabled in the scheduler so application must be started
// AFTER the scheduler because we need interrupts enabled when we run the
// application
void boardTestApp1Start()
{
    TaskHandle_t taskHandle;
    xTaskCreate(launcherTask, "Launcher", configMINIMAL_STACK_SIZE, NULL,
                                             tskIDLE_PRIORITY + 1, &taskHandle);
    vTaskStartScheduler();
    while (true) {
    }
}
