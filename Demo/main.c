#include <FreeRTOS.h>
#include <task.h>

#include "Drivers/interrupts.h"
#include "Drivers/gpio.h"

/* void task1(void *pParam) { */

/* 	int i = 0; */
/* 	while(1) { */
/* 		i++; */
/* 		SetGpio(16, 1); */
/* 		vTaskDelay(200); */
/* 	} */
/* } */

/* void task2(void *pParam) { */

/* 	int i = 0; */
/* 	while(1) { */
/* 		i++; */
/* 		vTaskDelay(100); */
/* 		SetGpio(16, 0); */
/* 		vTaskDelay(100); */
/* 	} */
/* } */

static inline void __my_delay(void){
  int i;

  for(i=0;i<10000/*00*/;i++)
    asm volatile("nop");
}

#define __dsb() __asm__ __volatile__ ("mcr   p15,0,%[t],c7,c10,4\n" :: [t] "r" (0) : "memory");

#define __isb(x) __asm__ __volatile__ ("mcr p15, 0, %0, c7, c5, 4" : : "r" (0) : "memory")
void taskSingle(void* param)
{
  int i;

  for(i=0;i<10;i++)
   {
      SetGpio(16, 0);
      uartPutS("\t--- task single  ---\n");
      __my_delay();
      /* __dsb(); */
      /* __isb(); */
      taskYIELD();
   }

  vTaskDelete(NULL);
}

void taskDuo(void* param){
  int i;
  for(i=0;i<10;i++) {
    SetGpio(16, 1);
    uartPutS("+++ task duo  +++\n");
    __my_delay();
      /* __dsb(); */
      /* __isb(); */
    taskYIELD();
  }

  vTaskDelete(NULL);
}
void taskEnd(void* param){
  uartPutS("before udf\n");
  
  asm volatile("udf");
  
  uartPutS("after udf\n");
}


/**
 *	This is the systems main entry, some call it a boot thread.
 *
 *	-- Absolutely nothing wrong with this being called main(), just it doesn't have
 *	-- the same prototype as you'd see in a linux program.
 **/
/* void print_cpu_mode(unsigned int psr) */
/* { */
/*   uartPutS("CPU Mode: "); */
/*   switch (psr & 0x1F) */
/*     { */
/*     case 0b10000: uartPutS("User"); break; */
/*     case 0b10001: uartPutS("FIQ"); break; */
/*     case 0b10010: uartPutS("IRQ"); break; */
/*     case 0b10011: uartPutS("Supervisor"); break; */
/*     case 0b10111: uartPutS("Abort"); break; */
/*     case 0b11011: uartPutS("Undefined"); break; */
/*     case 0b11111: uartPutS("System"); break; */
/*     case 0b10110: uartPutS("Monitor"); break; */
/*     default: uartPutS("Shut the fuck up"); */
/*     } */
/*   uartPutC('\n'); */
/* } */

/* unsigned int read_cpsr(void) */
/* { */
/*   unsigned int psr = -1; */
  
/*   asm volatile("mrs %0, cpsr" : : "r"(psr)); */

/*   return psr; */
/* } */

void main(void) {
  
  /* print_cpu_mode(read_cpsr()); */

  DisableInterrupts();
  InitInterruptController();

  /* SetGpioFunction(16, 1);			// RDY led */
  
  xTaskCreate(taskDuo, "Duo Task", 128, NULL, 1, NULL);
  xTaskCreate(taskSingle, "Single Task", 128, NULL, 1, NULL);
  //xTaskCreate(taskEnd, "End Task", 128, NULL, 0, NULL);
  uartPutS("Hop hop go working\n\n");
  vTaskStartScheduler();

  /*
   *	We should never get here, but just in case something goes wrong,
   *	we'll place the CPU into a safe loop.
   */
  while(1) {
    ;
  }
}
