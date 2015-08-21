#include <FreeRTOS.h>
#include "task.h"
#include "../../Demo/Drivers/uart.h"

void WRAPvTaskDelete( xTaskHandle pxTaskToDelete ){
  uartPutS("E Delete ");
  vTaskDelete( pxTaskToDelete );
  uartPutS("S Delete ");
}

void WRAPvTaskDelay( portTickType xTicksToDelay ){
  uartPutS("E Delay ");
  vTaskDelay(xTicksToDelay);
  uartPutS("S Delay ");
}
