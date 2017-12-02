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
 * 27.01.17 Arjun -- Alok
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
//#include "UART-interrupt-driven-command-console.h"

/* Dimensions the buffer into which input characters are placed. */
#define cmdMAX_INPUT_SIZE			50

/* Place holder for calls to ioctl that don't use the value parameter. */
#define cmdPARAMTER_NOT_USED		( ( void * ) 0 )

/* Block times of 50 and 500milliseconds, specified in ticks. */
#define cmd50ms						( ( void * ) ( 50UL / portTICK_RATE_MS ) )
#define cmd500ms					( ( void * ) ( 500UL / portTICK_RATE_MS ) )
/*-----------------------------------------------------------*/
static void prvUARTMyConsoleTask( void *pvParameters );
static const int8_t * const pcNewLine = ( int8_t * ) "\r\n";
static Peripheral_Descriptor_t xConsoleUART = NULL;
static xTaskHandle xCommandConsoleTask = NULL;

//extern uint32_t temperatureCaptured;
extern QueueHandle_t xGlobalQueue;
extern QueueHandle_t xGlobalQueue_UART;

typedef struct
{
	char ucMessageID;
	unsigned int ucTempValue;
	unsigned int ucLevelValue;
} xQueueUARTData;

static const int8_t * const pcMyMessage = ( int8_t * ) "\033[20;20HReal Time Gas Station Monitoring System\r\n\r\n";
int TankLevel,TankTemp;
void vUARTMyConsoleStart( void )
{
	//TankTemp=(temperatureCaptured * 0.125);
	xTaskCreate( 	prvUARTMyConsoleTask,				/* The task that implements the command console. */
			( const int8_t * const ) "UARTMYCONSOLE",		/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
			configUART_COMMAND_CONSOLE_STACK_SIZE,	/* The size of the stack allocated to the task. */
			NULL,									/* The parameter is not used, so NULL is passed. */
			configUART_COMMAND_CONSOLE_TASK_PRIORITY,/* The priority allocated to the task. */
			&xCommandConsoleTask );					/* Used to store the handle to the created task. */
}
/*-----------------------------------------------------------*/
static void prvUARTMyConsoleTask( void *pvParameters )
{
	int8_t cRxedChar, cInputIndex = 0, *pcOutputString;
	static int8_t cInputString[ cmdMAX_INPUT_SIZE ], cLastInputString[ cmdMAX_INPUT_SIZE ];
	portBASE_TYPE xReturned;

	portBASE_TYPE xStatus;

	xQueueUARTData queue_uart_data;

	uint32_t temperatureCaptured=25;

	char tmp[10];

	char lvl[3];

	( void ) pvParameters;


	/*copying the global queue in the stack area of this task in privileged mode*/
	xQueueHandle xLocalQueueUart=xGlobalQueue_UART;


	/* Obtain the address of the output buffer.  Note there is no mutual
	exclusion on this buffer as it is assumed only one command console
	interface will be used at any one time. */
	//pcOutputString = FreeRTOS_CLIGetOutputBuffer();

	/* Open the UART port used for console input.  The second parameter
	(ulFlags) is not used in this case.  The default board rate is set by the
	boardDEFAULT_UART_BAUD parameter.  The baud rate can be changed using a
	FreeRTOS_ioctl() call with the ioctlSET_SPEED command. */
	xConsoleUART = FreeRTOS_open( boardCOMMAND_CONSOLE_UART, ( uint32_t ) cmdPARAMTER_NOT_USED );
	configASSERT( xConsoleUART );

	/* Change the Tx usage model from straight polled mode to use zero copy
	buffers with interrupts.  In this mode, the UART will transmit characters
	directly from the buffer passed to the FreeRTOS_write()	function. */
	//	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlUSE_ZERO_COPY_TX, cmdPARAMTER_NOT_USED );
	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlUSE_CHARACTER_QUEUE_TX, (void *)50 );
	configASSERT( xReturned );

	/* Change the Rx usage model from straight polled mode to use a character
	queue.  Character queue reception is appropriate in this case as characters
	can only be received as quickly as they can be typed, and need to be parsed
	character by character. */
	//	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlUSE_CHARACTER_QUEUE_RX, ( void * ) cmdMAX_INPUT_SIZE );
	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlUSE_CHARACTER_QUEUE_RX, ( void * ) cmdMAX_INPUT_SIZE );
	configASSERT( xReturned );

	/* By default, the UART interrupt priority will have been set to the lowest
	possible.  It must be kept at or below configMAX_LIBRARY_INTERRUPT_PRIORITY,
	but	can be raised above its default priority using a FreeRTOS_ioctl() call
	with the ioctlSET_INTERRUPT_PRIORITY command. */
	xReturned = FreeRTOS_ioctl( xConsoleUART, ioctlSET_INTERRUPT_PRIORITY, ( void * ) ( configMIN_LIBRARY_INTERRUPT_PRIORITY - 1 ) );
	configASSERT( xReturned );

	/* Send the welcome message. */
	FreeRTOS_write( xConsoleUART, "\033[18;18H****************************************************", 51 );
	FreeRTOS_write( xConsoleUART, pcMyMessage, strlen( ( char * ) pcMyMessage ) );
	FreeRTOS_write( xConsoleUART, "\033[25;18H****************************************************", 51 );
	FreeRTOS_write( xConsoleUART, "\033[19;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[20;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[21;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[22;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[23;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[24;18H#", 12 );

	FreeRTOS_write( xConsoleUART, "\033[19;60H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[20;60H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[21;60H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[22;60H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[23;60H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[24;60H#", 12 );

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

			FreeRTOS_write( xConsoleUART, "\033[23;22HTank Temp: ", 20);
			FreeRTOS_write( xConsoleUART, tmp, strlen( ( char * ) tmp ) );
			FreeRTOS_write( xConsoleUART, "     Tank Level: ", 17);
			FreeRTOS_write( xConsoleUART, lvl, strlen( ( char * ) lvl) );
			FreeRTOS_write( xConsoleUART, "%", 1);
			//vTaskDelay(100);
		}
	}
}
/*-----------------------------------------------------------*/
