/*
 * Description: This task will read the input provided by the brake pedal and will convey the information to ECM
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

extern bool ecmBrake;

static void prvMyBrakeTask( void *pvParameters );


void vMyBrakeTaskStart( void )
{
	xTaskCreate(prvMyBrakeTask,
			( const int8_t * const ) "Brake_System",
			configBRAKE_TASK_STACK_SIZE,
			NULL,
			configBRAKE_TASK_PRIORITY,
			NULL );
}

void prvMyBrakeTask( void *pvParameters )
{
	bool brake;
	portTickType xLastExecutionTime;
	Chip_GPIO_SetPinDIR(LPC_GPIO, 2, 12, false);  //Stating its any input pin
	for(;;)
	{
		brake=Chip_GPIO_ReadPortBit(LPC_GPIO,2,12);
		ecmBrake=brake;
		//vTaskDelayUntil( &xLastExecutionTime, 10/portTICK_RATE_MS );
		vTaskDelay(25);
	}
}
