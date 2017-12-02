/*
 * Description: This task will control the other tasks output provided and give data to display task
 * i.e Engine control module
 *
 * Date: 28/01/17
 *
 * Author: Alok, Arjun, Swapnil & Kalidas.
 *
 */

#include "string.h"
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+IO includes. */
#include "FreeRTOS_IO.h"


/* GCC specifics. */
#define mainALIGN_TO( x )				__attribute__((aligned(x)))


/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/*-----------------------------------------------------------*/
/* Data used by the 'reg test' tasks. -----------------------*/
/*-----------------------------------------------------------*/

/* Define the constants used to allocate the reg test task stacks.  Note that
that stack size is defined in words, not bytes. */
#define configCONTROL_TASK_STACK_SIZE	128
#define mainREG_TEST_STACK_ALIGNMENT	( configCONTROL_TASK_STACK_SIZE * sizeof( portSTACK_TYPE ) )

/* Declare the stacks that will be used by the reg test tasks.  The kernel will
automatically create an MPU region for the stack.  The stack alignment must
match its size, so if 128 words are reserved for the stack then it must be
aligned to ( 128 * 4 ) bytes. */
mainALIGN_TO( mainREG_TEST_STACK_ALIGNMENT ) static StackType_t xRegTestStack[ configCONTROL_TASK_STACK_SIZE ];


/*-----------------------------------------------------------*/
bool ecmBrake=FALSE;
bool ecmAccelerate=FALSE;
bool ecmStatusBrake=FALSE;
bool ecmStatusAccelerator=FALSE;

extern QueueHandle_t xGlobalQueue;
extern QueueHandle_t xGlobalDispQueue;

typedef struct
{
	char ucMessageID;		//message id A: Acceleration B: Break
	bool ucValue;	//value 1 or 0;

}xQueueData;

typedef struct
{
	char ucMessageID;		//message id A: Acceleration B: Break
	bool ucValue;	//value 1 or 0;

}xQueueDisplayData;

/*-----------------------------------------------------------*/
static void prvMyControlTask( void *pvParameters );

static volatile xTaskParameters xRegControlTaskParameters =
{
		prvMyControlTask,						/* pvTaskCode - the function that implements the task. */
		"CONTROL_TASK",					/* pcName			*/
		configCONTROL_TASK_STACK_SIZE,			/* usStackDepth		*/
		NULL,					/* pvParameters - this value is just to test that the parameter is being passed into the task correctly. */
		configCONTROL_TASK_PRIORITY|portPRIVILEGE_BIT,	/* uxPriority - note that this task is created with privileges to demonstrate one method of passing a queue handle into the task. */
		xRegTestStack,							/* puxStackBuffer - the array to use as the task stack, as declared above. */
		{										/* xRegions - this task does not use any non-stack data hence all members are zero. */
				/* Base address		Length		Parameters */
				{ 0x2009C000,				0x3FFF,			portMPU_REGION_READ_WRITE },
				{ 0x00,				0x00,			0x00 },
				{ 0x00,				0x00,			0x00 }
		}
};

void vMyControlTaskStart(void)
{
	xTaskCreateRestricted( &xRegControlTaskParameters, NULL );
}

/*-----------------------------------------------------------*/


void prvMyControlTask( void *pvParameters )
{

	xQueueData queue_data;
	xQueueDisplayData queue_display_data;

	bool condition1 = FALSE,condition2 = FALSE;

	xQueueHandle xLocalRxQueue=xGlobalQueue;
	xQueueHandle xLocalTxQueue=xGlobalDispQueue;

	portSWITCH_TO_USER_MODE();

//	volatile uint8_t foo[1400];
//				volatile int i;
//			//stsacj koverdlor3we
//			//	for(i=0;i<1400;i++)
//			//	{
//			//		foo[i]=i;
//			//	}

	//int foo = xGlobalQueue;


	for( ; ; )
	{

		//Chip_WWDT_Feed(NULL);

		if(xQueueReceive(xLocalRxQueue,&queue_data,5) == pdPASS)
		{

			if(queue_data.ucMessageID == 'A')
			{
				condition1 = queue_data.ucValue;
				queue_display_data.ucMessageID = 'A';
				if (queue_data.ucValue == FALSE)
				{
					queue_display_data.ucValue = TRUE;
					//ecmStatusAccelerator=TRUE;
				}
				else
				{
					queue_display_data.ucValue = FALSE;
					//ecmStatusAccelerator=FALSE;
				}
			}



			if( xQueueSendToBack( xLocalTxQueue, &queue_display_data, 10 ) != pdPASS )
			{
				/* Data could not be sent to the queue even after waiting 10 ticks. */

				//do something important here
			}

			if(queue_data.ucMessageID == 'B')
			{
				condition2 = queue_data.ucValue;
				queue_display_data.ucMessageID = 'B';
				if(queue_data.ucValue == FALSE)
				{
					queue_display_data.ucValue = TRUE;
					//ecmStatusBrake=TRUE;
				}
				else
				{
					queue_display_data.ucValue = FALSE;
					//ecmStatusBrake=FALSE;
				}
			}

			if( xQueueSendToBack( xLocalTxQueue, &queue_display_data, 10 ) != pdPASS )
			{
				/* Data could not be sent to the queue even after waiting 10 ticks. */

				//do something important here
			}

			if(condition1 == FALSE && condition2 == FALSE)
			{
				xQueueHandle foo = xGlobalQueue;
			}
		}

		//vTaskDelayUntil( &xLastExecutionTime, 100/portTICK_RATE_MS );
		vTaskDelay(25);
	}

}
/*-----------------------------------------------------------*/
