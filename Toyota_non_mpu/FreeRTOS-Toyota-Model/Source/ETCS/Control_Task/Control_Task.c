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

/*-----------------------------------------------------------*/
bool ecmBrake=FALSE;
bool ecmAccelerate=FALSE;
bool ecmStatusBrake=FALSE;
bool ecmStatusAccelerator=FALSE;
/*-----------------------------------------------------------*/
static void prvMyControlTask( void *pvParameters );

void vMyControlTaskStart(void)
{
	xTaskCreate( prvMyControlTask,
			( const int8_t * const ) "CONTROL_TASK",
			configCONTROL_TASK_STACK_SIZE,
			NULL,
			configCONTROL_TASK_PRIORITY,				/* The priority allocated to the task. */
			NULL );									/* A handle to the task being created is not required, so just pass in NULL. */
}

/*-----------------------------------------------------------*/


void prvMyControlTask( void *pvParameters )
{

	portTickType xLastExecutionTime;

//		volatile uint8_t foo[1400];
//		volatile int i;
//	//stsacj koverdlor3we
//		for(i=0;i<1400;i++)
//		{
//			foo[i]=i;
//		}


	for( ; ; )
	{

		if (ecmAccelerate)
		{
			ecmStatusAccelerator=FALSE;
		}
		else
		{
			ecmStatusAccelerator=TRUE;
		}

		if(ecmBrake)
		{
			ecmStatusBrake=FALSE;
		}
		else
		{
			ecmStatusBrake=TRUE;
		}
		//vTaskDelayUntil( &xLastExecutionTime, 100/portTICK_RATE_MS );
		vTaskDelay(25);
	}

}
/*-----------------------------------------------------------*/
