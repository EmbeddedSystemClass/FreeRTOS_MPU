
/*
 *  Created on: 26-Jan-2017
 *  Author: swapnil
 */
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTO+IO includes. */
#include "FreeRTOS_IO.h"

/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/

extern QueueHandle_t xGlobalQueue;
typedef struct
{
	char ucMessageID;
	unsigned int ucValue;
} xQueueData;

/*
 * The task that uses the I2C to communicate with the OLED.
 */
static void prvMyUltrasonicTask( void *pvParameters );


void vMyUltrasonicTaskStart( void )
{
	xTaskCreate(prvMyUltrasonicTask,								/* The task that uses the I2C peripheral. */
			( const int8_t * const ) "LEVEL",	/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
			configLEVEL_TASK_STACK_SIZE,				/* The size of the stack allocated to the task. */
			NULL,									/* The parameter is not used, so NULL is passed. */
			configLEVEL_TASK_PRIORITY,				/* The priority allocated to the task. */
			NULL );
}

/*-----------------------------------------------------------*/


void prvMyUltrasonicTask( void *pvParameters )
{
	portBASE_TYPE xStatus;

	xQueueData queue_data;

	( void ) pvParameters;

	/*copying the global queue in the stack area of this task in privileged mode*/
	xQueueHandle xLocalQueue=xGlobalQueue;

	Chip_GPIO_WriteDirBit(LPC_GPIO,0,10,true);
	Chip_GPIO_WriteDirBit(LPC_GPIO,0,11,false);


	Chip_GPIO_WritePortBit(LPC_GPIO,0,10,false);
	Chip_GPIO_WritePortBit(LPC_GPIO,0,11,false);


	uint32_t timerFreq;
	Chip_TIMER_Init(LPC_TIMER0);
	timerFreq = Chip_Clock_GetSystemClockRate();

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_PrescaleSet(LPC_TIMER0,96000);


	for( ;; )
	{

		Chip_GPIO_WritePortBit(LPC_GPIO,0,10,false);
		Chip_TIMER_Enable(LPC_TIMER0);

		while(Chip_GPIO_GetPinState(LPC_GPIO,0,11)!=true)
		{

		}
		long time=Chip_TIMER_ReadCount(LPC_TIMER0);
		uint32_t Distance =(340*100)/time;
		Chip_TIMER_Disable(LPC_TIMER0);

		queue_data.ucMessageID=1;
		queue_data.ucValue=Distance;


		/* Send the message to the queue, waiting for 10 ticks for space to become
		available if the queue is already full. */
		if( xQueueSendToBack( xLocalQueue, &queue_data, 10 ) != pdPASS )
		{
			/* Data could not be sent to the queue even after waiting 10 ticks. */

			//do something important here
		}

		vTaskDelay(25);
	}
}
/*-----------------------------------------------------------*/
