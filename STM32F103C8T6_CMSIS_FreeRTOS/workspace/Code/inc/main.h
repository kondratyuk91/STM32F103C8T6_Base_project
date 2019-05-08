/*************************************************** Library ***************************************************/

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "sysInit.h"
#include "libUSART.h"
#include "libPWM.h"
#include "libADC.h"
#include "libDAC.h"

#include <stdio.h>

/*************************************************** Defines ***************************************************/

xQueueHandle SendDataADC;

/********************************************** Use Tasks FreeRTOS *********************************************/
void vTaskLed1 (void *argument);
void vTaskButtonPA1 (void *argument);
void vTaskPWM(void *argument);
void vTaskConvADC(void *argument);
void vTaskNextionHMI(void *argument);
