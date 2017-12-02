/*******************COMMENTS*******************************************/
/*
 * The Interrupt handler is present in FreeRTOS+IO products-->Device-->uart.h
 *
 *To Configure any UART one MUST write the handler in this header.
 *If you fail to write the handler there can be initial fault which claims that processor cannot interpret the interrupt.
 * After That we need to tweak with boardCONFIGURE_UART_PINS in LPCXpresso17xx-base-board.h file, here we need to make addition for suitable uart.
 * proper Pin multiplexing is done here.
 *
 * Please for initial testing of our approach using Character queue Rx & Tx.
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

/* Example includes. */
#include "FreeRTOS_CLI.h"
#include "UART-interrupt-driven-command-console.h"


#include "custom_MPU_wrappers.h"

/* Dimensions the buffer into which input characters are placed. */
#define cmdMAX_INPUT_SIZE			50

/* Place holder for calls to ioctl that don't use the value parameter. */
#define cmdPARAMTER_NOT_USED		( ( void * ) 0 )

/* Block times of 50 and 500milliseconds, specified in ticks. */
#define cmd50ms						( ( void * ) ( 50UL / portTICK_RATE_MS ) )
#define cmd500ms					( ( void * ) ( 500UL / portTICK_RATE_MS ) )
/*-----------------------------------------------------------*/
static void prvUARTMyConsoleTask( void *pvParameters );




/* GCC specifics. */
#define mainALIGN_TO( x )				__attribute__((aligned(x)))


#define mainREG_TEST_STACK_SIZE_WORDS	128
#define mainREG_TEST_STACK_ALIGNMENT	( mainREG_TEST_STACK_SIZE_WORDS * sizeof( portSTACK_TYPE ) )

mainALIGN_TO( mainREG_TEST_STACK_ALIGNMENT ) static StackType_t xRegTest1Stack[ mainREG_TEST_STACK_SIZE_WORDS ];


static const int8_t * const pcNewLine = ( int8_t * ) "\r\n";
static Peripheral_Descriptor_t xConsoleUART = NULL;
static xTaskHandle xCommandConsoleTask = NULL;

//extern uint32_t temperatureCaptured;



//why no problem with this global variable????


static const  int8_t * const pcMyMessage = ( int8_t * ) "\033[20;20HReal Time Gas Station Monitoring System\r\n\r\n";
int TankLevel,TankTemp;



/* Fill in a xTaskParameters structure per reg test task to define the tasks. */
static volatile xTaskParameters xRegTest1Parameters =
{
		prvUARTMyConsoleTask,						/* pvTaskCode - the function that implements the task. */
		( const int8_t * const ) "UARTMYCONSOLE",					/* pcName			*/
		mainREG_TEST_STACK_SIZE_WORDS,			/* usStackDepth		*/
		NULL,					/* pvParameters - this value is just to test that the parameter is being passed into the task correctly. */
		2|portPRIVILEGE_BIT ,	/* uxPriority - note that this task is created with privileges to demonstrate one method of passing a queue handle into the task. */
		xRegTest1Stack,							/* puxStackBuffer - the array to use as the task stack, as declared above. */
		{										/* xRegions - this task does not use any non-stack data hence all members are zero. */
				/* Base address		Length		Parameters */
				//	{ my_variable,		64,			portMPU_REGION_READ_WRITE },
				{0x00,0x00,0x00},
				{ 0x00,				0x00,			0x00 },
				{ 0x00,				0x00,			0x00 }
		}
};




extern QueueHandle_t xGlobalQueue;
extern QueueHandle_t xGlobalQueue_UART;

typedef struct
{
	char ucMessageID;
	unsigned int ucTempValue;
	unsigned int ucLevelValue;
} xQueueUARTData;



/*-----------------------------------------------------------*/




void vUARTMyConsoleStart( void )
{

	//TankTemp=(temperatureCaptured * 0.125);


	xTaskCreateRestricted( &xRegTest1Parameters, NULL );

}
/*-----------------------------------------------------------*/
static void prvUARTMyConsoleTask( void *pvParameters )
{


	portBASE_TYPE xStatus;

	xQueueUARTData queue_uart_data;

	uint32_t temperatureCaptured=25;

	char tmp[10];
	char lvl[10];


	void * my_ConsoleUART=NULL;
	int8_t cRxedChar = 0, cInputIndex = 0, *pcOutputString;
	static int8_t cInputString[ cmdMAX_INPUT_SIZE ], cLastInputString[ cmdMAX_INPUT_SIZE ];
	portBASE_TYPE xReturned;

	( void ) pvParameters;



	/*copying the global queue in the stack area of this task in privileged mode*/
	xQueueHandle xLocalQueueUart=xGlobalQueue_UART;


	//pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	xConsoleUART = FreeRTOS_open( boardCOMMAND_CONSOLE_UART, ( uint32_t ) cmdPARAMTER_NOT_USED );
	configASSERT( xConsoleUART );


	my_ConsoleUART=xConsoleUART;


	portSWITCH_TO_USER_MODE();



	xReturned = FreeRTOS_ioctl( my_ConsoleUART, ioctlUSE_CHARACTER_QUEUE_TX, (void *)50 );
	configASSERT( xReturned );


	xReturned = FreeRTOS_ioctl( my_ConsoleUART, ioctlUSE_CHARACTER_QUEUE_RX, ( void * ) cmdMAX_INPUT_SIZE );
	configASSERT( xReturned );


	xReturned = FreeRTOS_ioctl( my_ConsoleUART, ioctlSET_INTERRUPT_PRIORITY, ( void * ) ( configMIN_LIBRARY_INTERRUPT_PRIORITY - 1 ) );
	configASSERT( xReturned );

	/* Send the welcome message. */
	//	if( FreeRTOS_ioctl( xConsoleUART, ioctlOBTAIN_WRITE_MUTEX, cmd50ms ) == pdPASS )
	{
		FreeRTOS_write( my_ConsoleUART, "\033[18;18H****************************************************", 51 );
		FreeRTOS_write( my_ConsoleUART, pcMyMessage, strlen( ( char * ) pcMyMessage ) );
		FreeRTOS_write( my_ConsoleUART, "\033[25;18H****************************************************", 51 );
		FreeRTOS_write( my_ConsoleUART, "\033[19;18H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[20;18H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[21;18H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[22;18H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[23;18H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[24;18H#", 12 );

		FreeRTOS_write( my_ConsoleUART, "\033[19;60H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[20;60H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[21;60H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[22;60H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[23;60H#", 12 );
		FreeRTOS_write( my_ConsoleUART, "\033[24;60H#", 12 );
	}

	for( ;; )
	{


		if( xQueueReceive( xLocalQueueUart, &queue_uart_data, 10 ) != pdPASS )
		{
			/* Nothing was received from the queue – even after blocking to wait
		for data to arrive. */
		}
		else
		{
			if( queue_uart_data.ucMessageID == 5 )
			{
				sprintf(tmp,"%d",queue_uart_data.ucTempValue);

				sprintf(lvl,"%d",queue_uart_data.ucLevelValue);
			}

			FreeRTOS_write( my_ConsoleUART, "\033[23;22HTank Temp: ", 20);
			FreeRTOS_write( my_ConsoleUART, tmp, strlen( ( char * ) tmp ) );
			FreeRTOS_write( my_ConsoleUART, "     Tank Level: ", 17);
			FreeRTOS_write( my_ConsoleUART, lvl, strlen( ( char * ) lvl) );
			FreeRTOS_write( my_ConsoleUART, "%", 1);


			/* xMessage now contains the received data. */
		}


	}
	//}
}
/*-----------------------------------------------------------*/
