#include "FreeRTOS.h"
#include "boardTestAppStart.h"
#include "boardTestApp.h"
#include "libRaisePrivilege.h"

void boardTestAppStart()
{
    LibRaisePrivilege libRaisePrivilege;
    BoardTestApp boardTestApp("BoardTestApp");
    boardTestApp.start();
    vTaskStartScheduler();
    while (true) {
    }
}
