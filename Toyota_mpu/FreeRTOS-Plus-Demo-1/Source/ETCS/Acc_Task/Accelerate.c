/*
 * Description: This task will read the input provided by the Acceleration Pedal and will convey the information to ECM
 * i.e Engine control module
 *
 * Date: 28/01/17
 *
 * Author: Alok, Arjun, Swapnil & Kalidas.
 *
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTO+IO includes. */
#include "FreeRTOS_IO.h"

/*-----------------------------------------------------------*/

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
#define configACCELERATION_TASK_STACK_SIZE	128
#define mainREG_TEST_STACK_ALIGNMENT	( configACCELERATION_TASK_STACK_SIZE * sizeof( portSTACK_TYPE ) )

/* Declare the stacks that will be used by the reg test tasks.  The kernel will
automatically create an MPU region for the stack.  The stack alignment must
match its size, so if 128 words are reserved for the stack then it must be
aligned to ( 128 * 4 ) bytes. */
mainALIGN_TO( mainREG_TEST_STACK_ALIGNMENT ) static StackType_t xRegTestStack[ configACCELERATION_TASK_STACK_SIZE ];



extern bool ecmAccelerate;
extern QueueHandle_t xGlobalQueue;
typedef struct
{
	char ucMessageID;		//message id A: Acceleration B: Break
	bool ucValue;	//value 1 or 0;

}xQueueData;

static void prvMyAccelerationTask( void *pvParameters );

static volatile xTaskParameters xRegAccelerationTaskParameters =
{
		prvMyAccelerationTask,						/* pvTaskCode - the function that implements the task. */
		"Accelerate",					/* pcName			*/
		configACCELERATION_TASK_STACK_SIZE,			/* usStackDepth		*/
		NULL,					/* pvParameters - this value is just to test that the parameter is being passed into the task correctly. */
		configACCELERATION_TASK_PRIORITY|portPRIVILEGE_BIT,	/* uxPriority - note that this task is created with privileges to demonstrate one method of passing a queue handle into the task. */
		xRegTestStack,							/* puxStackBuffer - the array to use as the task stack, as declared above. */
		{										/* xRegions - this task does not use any non-stack data hence all members are zero. */
				/* Base address		Length		Parameters */
				{ 0x2009C000,				0x3FFF,			portMPU_REGION_READ_WRITE },
				{ 0x00,				0x00,			0x00 },
				{ 0x00,				0x00,			0x00 }
		}
};


void vMyAccelerationTaskStart( void )
{
	xTaskCreateRestricted( &xRegAccelerationTaskParameters, NULL );
}

void prvMyAccelerationTask( void *pvParameters )
{
	bool Accelerate;
	xQueueData Accelerate_data;
	portTickType xLastExecutionTime;
	Chip_GPIO_SetPinDIR(LPC_GPIO, 0, 4, false);

	xQueueHandle xLocalQueue=xGlobalQueue;
	portSWITCH_TO_USER_MODE();

	for(;;)
	{
		//Chip_WWDT_Feed(NULL);

		Accelerate=Chip_GPIO_ReadPortBit(LPC_GPIO,0,4);
		//ecmAccelerate=Accelerate;

		Accelerate_data.ucMessageID = 'A';
		Accelerate_data.ucValue = Accelerate;

		if( xQueueSendToBack( xLocalQueue, &Accelerate_data, 10 ) != pdPASS )
		{
			/* Data could not be sent to the queue even after waiting 10 ticks. */

			//do something important here
		}

		//	vTaskDelayUntil( &xLastExecutionTime, 10/portTICK_RATE_MS );
		vTaskDelay(25);
	}
}
