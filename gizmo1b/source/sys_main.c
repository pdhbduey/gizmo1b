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
#include <stdlib.h>
#include <string.h>
#include <FreeRTOS.h>
#include <boardTestApp2Start.h>
#include <BoardTestApp1Start.h>
#include <BoardTestConsoleApp1Start.h>
#include <BoardTestConsoleApp2Start.h>
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
#if defined BB_GUI_APP
    boardTestApp1Start();
#elif defined TB_GUI_APP
    boardTestApp2Start();
#elif defined BB_TERMINAL_APP
    boardTestConsoleApp1Start();
#elif defined TB_TERMINAL_APP
    boardTestConsoleApp2Start();
#else
#error Must define one of: BB_GUI_APP, TB_GUI_APP, BB_TERMINAL_APP, TB_TERMINAL_APP
#endif
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void* calloc(size_t _num, size_t _size)
{
    void* p = pvPortMalloc(_num * _size);
    memset(p, 0, _num * _size);
    return p;
}

void* malloc(size_t size)
{
     void* p = pvPortMalloc(size);
     return p;
}

void* realloc(void* _ptr, size_t _size)
{
    void* p = pvPortMalloc(_size);
    memcpy(p, _ptr, _size);
    vPortFree(_ptr);
    return p;
}

void free(void* _ptr)
{
    vPortFree(_ptr);
}

void* memalign(size_t _aln, size_t _size)
{
    abort();
    return NULL;
}
/* USER CODE END */
