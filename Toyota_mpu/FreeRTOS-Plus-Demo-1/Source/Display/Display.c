/*
 * Description: This task is used for infotainment will convey the information to ECM
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
#define configDISPLAY_TASK_STACK_SIZE	128
#define mainREG_TEST_STACK_ALIGNMENT	( configDISPLAY_TASK_STACK_SIZE * sizeof( portSTACK_TYPE ) )

/* Declare the stacks that will be used by the reg test tasks.  The kernel will
automatically create an MPU region for the stack.  The stack alignment must
match its size, so if 128 words are reserved for the stack then it must be
aligned to ( 128 * 4 ) bytes. */
mainALIGN_TO( mainREG_TEST_STACK_ALIGNMENT ) static StackType_t xRegTestStack[ configDISPLAY_TASK_STACK_SIZE ];


extern bool ecmStatusBrake;
extern bool ecmStatusAccelerator;

TaskHandle_t xLCDTask = NULL;

extern QueueHandle_t xGlobalDispQueue;

typedef struct
{
	char ucMessageID;		//message id A: Acceleration B: Break
	bool ucValue;	//value 1 or 0;

}xQueueDisplayData;

static void prvMyDisplayTask( void *pvParameters );

static volatile xTaskParameters xRegDisplayTaskParameters =
{
		prvMyDisplayTask,						/* pvTaskCode - the function that implements the task. */
		"Display",					/* pcName			*/
		configDISPLAY_TASK_STACK_SIZE,			/* usStackDepth		*/
		NULL,					/* pvParameters - this value is just to test that the parameter is being passed into the task correctly. */
		configDISPLAY_TASK_PRIORITY|portPRIVILEGE_BIT,	/* uxPriority - note that this task is created with privileges to demonstrate one method of passing a queue handle into the task. */
		xRegTestStack,							/* puxStackBuffer - the array to use as the task stack, as declared above. */
		{										/* xRegions - this task does not use any non-stack data hence all members are zero. */
				/* Base address		Length		Parameters */
				{ 0x2009C000,				0x3FFF,			portMPU_REGION_READ_WRITE },
				{ 0x00,				0x00,			0x00 },
				{ 0x00,				0x00,			0x00 }
		}
};


void vMyDisplayStart( void )
{
	//LCD initialization
	Board_LCD_Init();

	xTaskCreateRestricted( &xRegDisplayTaskParameters, &xLCDTask );
}

void prvMyDisplayTask( void *pvParameters )
{

	xQueueDisplayData queue_display_data;

	xQueueHandle xLocalRxQueue=xGlobalDispQueue;


	portSWITCH_TO_USER_MODE();


	portTickType xLastExecutionTime;
	Board_Lcd_Cmd_Write(0x80|1);
	Board_LCD_Write_String("Engine Status");
	Board_Lcd_Cmd_Write(0xc0);
	Board_LCD_Write_String("Acc:");
	Board_Lcd_Cmd_Write(0xc0|10);
	Board_LCD_Write_String("Br:");


	for(;;)
	{

		//Chip_WWDT_Feed(NULL);

		if(xQueueReceive(xLocalRxQueue,&queue_display_data,5) == pdPASS)
		{

			if(queue_display_data.ucMessageID == 'A')
			{
				if(queue_display_data.ucValue == TRUE)
				{
					Board_Lcd_Cmd_Write(0xc0|5);
					Board_LCD_Write_String("ON ");
				}
				else
				{
					Board_Lcd_Cmd_Write(0xc0|5);
					Board_LCD_Write_String("OFF");
				}
			}

			if(queue_display_data.ucMessageID == 'B')
			{
				if(queue_display_data.ucValue == TRUE)
				{
					Board_Lcd_Cmd_Write(0xc0|13);
					Board_LCD_Write_String("ON ");
				}
				else
				{
					Board_Lcd_Cmd_Write(0xc0|13);
					Board_LCD_Write_String("OFF");
				}
			}
			//	vTaskDelayUntil( &xLastExecutionTime, 10/portTICK_RATE_MS );
			//vTaskDelay(25);
		}
	}
}
