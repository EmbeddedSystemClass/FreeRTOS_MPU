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
extern bool ecmStatusBrake;
extern bool ecmStatusAccelerator;

static void prvMyDisplayTask( void *pvParameters );


void vMyDisplayStart( void )
{
	//LCD initialization
	Board_LCD_Init();
	xTaskCreate(prvMyDisplayTask,
			( const int8_t * const ) "Display",
			configDISPLAY_TASK_STACK_SIZE,
			NULL,
			configDISPLAY_TASK_PRIORITY,
			NULL );
}

void prvMyDisplayTask( void *pvParameters )
{
	portTickType xLastExecutionTime;
	Board_Lcd_Cmd_Write(0x80|1);
	Board_LCD_Write_String("Engine Status");
	Board_Lcd_Cmd_Write(0xc0);
	Board_LCD_Write_String("Acc:");
	Board_Lcd_Cmd_Write(0xc0|10);
	Board_LCD_Write_String("Br:");
	for(;;)
	{
		if(ecmStatusAccelerator==TRUE && ecmStatusBrake==TRUE) //Rare Condition & this will happen cause unintended acceleration
		{
			Board_Lcd_Cmd_Write(0xc0|5);
			Board_LCD_Write_String("ON ");
			Board_Lcd_Cmd_Write(0xc0|13);
			Board_LCD_Write_String("ON ");
		}
		else if(ecmStatusAccelerator==TRUE && ecmStatusBrake==FALSE) //Accelerator pedal is pressed
		{
			Board_Lcd_Cmd_Write(0xc0|5);
			Board_LCD_Write_String("ON ");
			Board_Lcd_Cmd_Write(0xc0|13);
			Board_LCD_Write_String("OFF");
		}
		else if(ecmStatusAccelerator==FALSE && ecmStatusBrake==TRUE) //Brake pedal is pressed
		{
			Board_Lcd_Cmd_Write(0xc0|5);
			Board_LCD_Write_String("OFF");
			Board_Lcd_Cmd_Write(0xc0|13);
			Board_LCD_Write_String("ON ");
		}
		else //Both the pedals are released
		{
			Board_Lcd_Cmd_Write(0xc0|5);
			Board_LCD_Write_String("OFF");
			Board_Lcd_Cmd_Write(0xc0|13);
			Board_LCD_Write_String("OFF");
		}
		//	vTaskDelayUntil( &xLastExecutionTime, 10/portTICK_RATE_MS );
		//vTaskDelay(25);
	}
}
