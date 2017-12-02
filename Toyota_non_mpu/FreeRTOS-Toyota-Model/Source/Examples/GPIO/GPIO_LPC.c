#include "FreeRTOS.h"
#include "FreeRTOS_IO.h"
#include "task.h"
//LPC chip library
#include "gpio_17xx_40xx.h"
void GPIO_LED_BLINK(void *);

void vMyLEDStart( void )
{
	//SystemCoreClockUpdate();
	Board_Init();
	xTaskCreate( 	GPIO_LED_BLINK,				/* The task that implements the command console. */
					"LED TAsk",		/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
					240,	/* The size of the stack allocated to the task. */
					NULL,									/* The parameter is not used, so NULL is passed. */
					2,/* The priority allocated to the task. */
					NULL);					/* Used to store the handle to the created task. */
}
void GPIO_LED_BLINK(void *pvParameters)
{
//	LPC_GPIO_T *pGPIO;
//	Chip_GPIO_Init(*pGPIO);
//	Chip_GPIO_SetPinDIROutput(pGPIO, 0, 10);
	while(1){
	//	Chip_GPIO_WritePortBit(pGPIO, 0, 10, 1);
			//Board_LED_Set(0,false);
			//for(int i=0;i<100000;i++)
				//for(int j=0;j<100;j++);
		vTaskDelay(500);
			//Board_LED_Set(0,true);
					Board_LED_Toggle(0);
	//	Chip_GPIO_WritePortBit(pGPIO, 0, 10, 0);
		}
}
