#ifndef WRAP_H
#define WRAP_H

#include <FreeRTOS.h>
#include <task.h>

void WRAPvTaskDelete( xTaskHandle pxTaskToDelete );
void WRAPvTaskDelay( portTickType xTicksToDelay );

#endif
