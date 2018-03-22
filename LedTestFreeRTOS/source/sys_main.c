/** @file sys_main.c 
*   @brief Application main file
*   @date 07-July-2017
*   @version 04.07.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_semphr.h"
#include "mibspi.h"
#include "gio.h"
#include "sys_pmu.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define US_TO_CYCLES(us) (us * 160)

void delay_cycles (const uint32 delay)
{
    const uint32 start = _pmuGetCycleCount_ ();
    while ((_pmuGetCycleCount_ () - start) < delay)
    {
    }
}

/* Priorities at which the tasks are created. */
#define mainMALLOC_TASK_PRIORITY            ( tskIDLE_PRIORITY + 3 )
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The 200ms value is converted
to ticks using the portTICK_PERIOD_MS constant. */
#define mainQUEUE_SEND_FREQUENCY_MS         ( 200 / portTICK_PERIOD_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH                    ( 1 )

/* Values passed to the two tasks just to check the task parameter
functionality. */
#define mainQUEUE_SEND_PARAMETER            ( 0x1111UL )
#define mainQUEUE_RECEIVE_PARAMETER         ( 0x22UL )


/* The rate at which malloc task is woken up. The 200ms value is converted
to ticks using the portTICK_PERIOD_MS constant. */
#define mainMALLOC_FREQUENCY_MS         ( 200 / portTICK_PERIOD_MS )

#define portRESET_PRIVILEGE(xRunningPrivileged) \
                         if( xRunningPrivileged == 0 ) portSWITCH_TO_USER_MODE()
#pragma SWI_ALIAS(prvRaisePrivilege, 1)
extern BaseType_t prvRaisePrivilege(void);

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvMallocTask( void *pvParameters );
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/*
 * Called by main() to create the simply blinky style application if
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1.
 */
void main_blinky( void );

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/*-----------------------------------------------------------*/

void main_blinky( void )
{
    /* Create the queue. */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( unsigned long ) );

    if( xQueue != NULL )
    {
        /* Start the two tasks as described in the comments at the top of this
        file. */
        xTaskCreate( prvQueueReceiveTask,                   /* The function that implements the task. */
                    "Rx",                                   /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                    configMINIMAL_STACK_SIZE,               /* The size of the stack to allocate to the task. */
                    ( void * ) mainQUEUE_RECEIVE_PARAMETER, /* The parameter passed to the task - just to check the functionality. */
                    mainQUEUE_RECEIVE_TASK_PRIORITY,        /* The priority assigned to the task. */
                    NULL );                                 /* The task handle is not required, so NULL is passed. */

        xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE,
                                           ( void * ) mainQUEUE_SEND_PARAMETER,
                                           mainQUEUE_SEND_TASK_PRIORITY, NULL );
        xTaskCreate( prvMallocTask, "MALLOC", configMINIMAL_STACK_SIZE, NULL,
                                               mainMALLOC_TASK_PRIORITY, NULL );

        /* Start the tasks and timer running. */
        vTaskStartScheduler();
    }

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the idle and/or
    timer tasks to be created.  See the memory management section on the
    FreeRTOS web site for more details. */
    for( ;; );
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask( void *pvParameters )
{
    TickType_t xNextWakeTime;
    const unsigned long ulValueToSend = 100UL;

    /* Check the task parameter is as expected. */
    configASSERT( ( ( unsigned long ) pvParameters ) == mainQUEUE_SEND_PARAMETER );

    /* Initialise xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        /* Place this task in the blocked state until it is time to run again.
        The block time is specified in ticks, the constant used converts ticks
        to ms.  While in the Blocked state this task will not consume any CPU
        time. */
        vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

        /* Send to the queue - causing the queue receive task to unblock and
        toggle the LED.  0 is used as the block time so the sending operation
        will not block - it shouldn't need to block as the queue should always
        be empty at this point in the code. */
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
    unsigned long ulReceivedValue;

    /* Check the task parameter is as expected. */
    configASSERT( ( ( unsigned long ) pvParameters ) == mainQUEUE_RECEIVE_PARAMETER );

    for( ;; )
    {
        /* Wait until something arrives in the queue - this task will block
        indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h. */
        xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

        /*  To get here something must have been received from the queue, but
        is it the expected value?  If it is, toggle the LED. */
        if( ulReceivedValue == 100UL )
        {
            gioToggleBit(mibspiPORT1, PIN_ENA);
            gioToggleBit(mibspiPORT1, PIN_CS0);
            ulReceivedValue = 0U;
        }
    }
}
/*-----------------------------------------------------------*/

static void prvMallocTask( void *pvParameters )
{
    /* Initialise xNextWakeTime - this only needs to be done once. */
    TickType_t xNextWakeTime = xTaskGetTickCount();
    void* p = NULL;

    for( ;; )
    {
        /* Place this task in the blocked state until it is time to run again.
        The block time is specified in ticks, the constant used converts ticks
        to ms.  While in the Blocked state this task will not consume any CPU
        time. */
        vTaskDelayUntil( &xNextWakeTime, mainMALLOC_FREQUENCY_MS );
        BaseType_t xRunningPrivileged = prvRaisePrivilege();

        if (p) {
            vPortFree(p);
            p = NULL;
        }
        else {
            p = pvPortMalloc(1024);
        }
        portRESET_PRIVILEGE( xRunningPrivileged );
    }
}
/*-----------------------------------------------------------*/

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    mibspiInit();
    gioInit();
    _pmuEnableCountersGlobal_ ();
    _pmuStartCounters_(pmuCYCLE_COUNTER);
    gioSetBit(mibspiPORT1, PIN_ENA, 1);
    gioSetBit(mibspiPORT1, PIN_CS0, 0);
//    while (1) {
//        gioToggleBit(mibspiPORT1, PIN_ENA);
//        gioToggleBit(mibspiPORT1, PIN_CS0);
//        delay_cycles(US_TO_CYCLES(500000));
//    }
    main_blinky();
#if 0
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
#endif
}
/* USER CODE END */
