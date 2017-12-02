
/*
 * control_task.c
 *
 *  Created on: 26-Jan-2017
 *      Author: swapnil
 */
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTO+IO includes. */
#include "FreeRTOS_IO.h"

#include "custom_MPU_wrappers.h"
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




#define mainALIGN_TO( x )				__attribute__((aligned(x)))


#define mainREG_TEST3_STACK_SIZE_WORDS	128
#define mainREG_TEST3_STACK_ALIGNMENT	( mainREG_TEST3_STACK_SIZE_WORDS * sizeof( portSTACK_TYPE ) )

mainALIGN_TO( mainREG_TEST3_STACK_ALIGNMENT ) static StackType_t xRegTest3Stack[ mainREG_TEST3_STACK_SIZE_WORDS ];



/* Fill in a xTaskParameters structure per reg test task to define the tasks. */
static volatile xTaskParameters xRegTest3Parameters =
{
		prvMyUltrasonicTask,						/* pvTaskCode - the function that implements the task. */
		( const int8_t * const ) "Ultrasonic",					/* pcName			*/
		mainREG_TEST3_STACK_SIZE_WORDS,			/* usStackDepth		*/
		NULL,					/* pvParameters - this value is just to test that the parameter is being passed into the task correctly. */
		3|portPRIVILEGE_BIT ,	/* uxPriority - note that this task is created with privileges to demonstrate one method of passing a queue handle into the task. */
		xRegTest3Stack,							/* puxStackBuffer - the array to use as the task stack, as declared above. */
		{										/* xRegions - this task does not use any non-stack data hence all members are zero. */
				/* Base address		Length		Parameters */
				//	{ my_variable,		64,			portMPU_REGION_READ_WRITE },
				{0x00,0x00,0x00},
				{ 0x00,				0x00,			0x00 },
				{ 0x00,				0x00,			0x00 }
		}
};


void vMyUltrasonicTaskStart( void )
{
	xTaskCreateRestricted( &xRegTest3Parameters, NULL );
}

/*-----------------------------------------------------------*/






void prvMyUltrasonicTask( void *pvParameters )
{
	portBASE_TYPE xStatus;

	xQueueData queue_data;



	( void ) pvParameters;


	/*copying the global queue in the stack area of this task in privileged mode*/
	xQueueHandle xLocalQueue=xGlobalQueue;




	Chip_IOCON_PinMux(LPC_IOCON, 1, 23, IOCON_MODE_INACT, IOCON_FUNC0);
	Chip_IOCON_PinMux(LPC_IOCON, 1, 24, IOCON_MODE_INACT, IOCON_FUNC0);

	Chip_GPIO_WriteDirBit(LPC_GPIO,1,23,true);

	Chip_GPIO_WriteDirBit(LPC_GPIO,1,24,false);






	uint32_t timerFreq;
	Chip_TIMER_Init(LPC_TIMER0);
	timerFreq = Chip_Clock_GetSystemClockRate();

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_PrescaleSet(LPC_TIMER0,96000);



	//portSWITCH_TO_USER_MODE();

	for( ;; )
	{
		Chip_GPIO_WritePortBit(LPC_GPIO,1,23,false);
		Chip_GPIO_WritePortBit(LPC_GPIO,1,23,true);
		Chip_TIMER_Enable(LPC_TIMER0);

		while(Chip_GPIO_GetPinState(LPC_GPIO,1,24)!=true)
		{

		}


		long time=Chip_TIMER_ReadCount(LPC_TIMER0);
		uint32_t Distance =(340*time*100)/1000;
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
