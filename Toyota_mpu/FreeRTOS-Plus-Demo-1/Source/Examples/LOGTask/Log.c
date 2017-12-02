/*
 * Log.c
 *
 *  Created on: 29-Jan-2017
 *      Author: swapnil
 */
/*
 * control_task.c
 *
 *  Created on: 26-Jan-2017
 *      Author: swapnil
 */
#include "string.h"
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+IO includes. */
#include "FreeRTOS_IO.h"

#include "custom_MPU_wrappers.h"
/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/


static void prvMyLogTask( void *pvParameters );


volatile char *pcWriteBuffer;

#define mainALIGN_TO( x )				__attribute__((aligned(x)))


#define mainREG_TEST4_STACK_SIZE_WORDS	128
#define mainREG_TEST4_STACK_ALIGNMENT	( mainREG_TEST4_STACK_SIZE_WORDS * sizeof( portSTACK_TYPE ) )

mainALIGN_TO( mainREG_TEST4_STACK_ALIGNMENT ) static StackType_t xRegTEST4Stack[ mainREG_TEST4_STACK_SIZE_WORDS ];



/* Fill in a xTaskParameters structure per reg test task to define the tasks. */
static volatile xTaskParameters xRegTEST4Parameters =
{
		prvMyLogTask,						/* pvTaskCode - the function that implements the task. */
		( const char *) "Log",		/* pcName			*/
		mainREG_TEST4_STACK_SIZE_WORDS,			/* usStackDepth		*/
		NULL,					/* pvParameters - this value is just to test that the parameter is being passed into the task correctly. */
		3|portPRIVILEGE_BIT ,	/* uxPriority - note that this task is created with privileges to demonstrate one method of passing a queue handle into the task. */
		xRegTEST4Stack,							/* puxStackBuffer - the array to use as the task stack, as declared above. */
		{										/* xRegions - this task does not use any non-stack data hence all members are zero. */
				/* Base address		Length		Parameters */
				//	{ my_variable,		64,			portMPU_REGION_READ_WRITE },
				{0x00,0x00,0x00},
				{ 0x00,				0x00,			0x00 },
				{ 0x00,				0x00,			0x00 }
		}
};


void vMyLogDataTaskStart( void )
{
	xTaskCreateRestricted( &xRegTEST4Parameters, NULL );
}

/*-----------------------------------------------------------*/






void prvMyLogTask( void *pvParameters )
{
	portTickType xLastExecutionTime;

	portBASE_TYPE xStatus;

	pcWriteBuffer=pvPortMalloc(100*sizeof(char));

	( void ) pvParameters;


	xLastExecutionTime = xTaskGetTickCount();
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	uint32_t ulTotalTime, ulStatsAsPercentage;


	for( ;; )
	{
		//vTaskGetRunTimeStats(pcWriteBuffer);
		//vTaskList( pcWriteBuffer );
//
//		uxArraySize = uxCurrentNumberOfTasks;
//		pxTaskStatusArray = pvPortMalloc( uxCurrentNumberOfTasks * sizeof( TaskStatus_t ) );



		//uxTaskGetSystemState();

		vTaskDelayUntil( &xLastExecutionTime, 10/portTICK_RATE_MS );
	}
}
/*-----------------------------------------------------------*/
