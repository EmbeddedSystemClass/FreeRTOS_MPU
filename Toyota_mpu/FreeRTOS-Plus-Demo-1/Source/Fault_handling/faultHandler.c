/*
 * faultHandler.c
 *
 *  Created on: 29 Jan 2017
 *      Author: Kalidas Tate
 */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_IO.h"
extern Peripheral_Descriptor_t xConsoleUART1;
extern xTaskHandle xCommandConsoleTask;

void pop_registers_from_fault_stack(unsigned int * hardfault_args)
{
unsigned int stacked_r0;
unsigned int stacked_r1;
unsigned int stacked_r2;
unsigned int stacked_r3;
unsigned int stacked_r12;
unsigned int stacked_lr;
volatile unsigned int stacked_pc;
unsigned int stacked_psr;

	stacked_r0 = ((unsigned long) hardfault_args[0]);
	stacked_r1 = ((unsigned long) hardfault_args[1]);
	stacked_r2 = ((unsigned long) hardfault_args[2]);
	stacked_r3 = ((unsigned long) hardfault_args[3]);

	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr = ((unsigned long) hardfault_args[5]);
	stacked_pc = ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);


	SCB->CFSR |= (1<<1)|(1<<7);	//  Data access voilation & MMFAR valid bit
	__set_CONTROL(0x00);

	 vTaskDelete(xCommandConsoleTask);

	 xTaskResumeFromISR(xConsoleUART1);
	/* Inspect stacked_pc to locate the0x10004008 offending instruction. */

	//for( ;; );
}

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE
