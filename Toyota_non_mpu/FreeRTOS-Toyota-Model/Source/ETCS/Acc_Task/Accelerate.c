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

extern bool ecmAccelerate;

static void prvMyAccelerationTask( void *pvParameters );


void vMyAccelerationTaskStart( void )
{
	xTaskCreate(prvMyAccelerationTask,
			( const int8_t * const ) "Accelerate",
			configACCELERATION_TASK_STACK_SIZE,
			NULL,
			configACCELERATION_TASK_PRIORITY,
			NULL );
}

void prvMyAccelerationTask( void *pvParameters )
{
	bool Accelerate;
	portTickType xLastExecutionTime;
	Chip_GPIO_SetPinDIR(LPC_GPIO, 0, 4, false);
	for(;;)
	{
		Accelerate=Chip_GPIO_ReadPortBit(LPC_GPIO,0,4);
		ecmAccelerate=Accelerate;
	//	vTaskDelayUntil( &xLastExecutionTime, 10/portTICK_RATE_MS );
		vTaskDelay(25);
	}
}
