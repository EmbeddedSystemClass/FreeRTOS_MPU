/*
 * Description:
 * The Interrupt handler is present in FreeRTOS+IO products-->Device-->uart.h
 *To Configure any UART one MUST write the handler in this header.
 *If you fail to write the handler there can be initial fault which claims that processor cannot interpret the interrupt.
 * After That we need to tweak with boardCONFIGURE_UART_PINS in LPCXpresso17xx-base-board.h file, here we need to make addition for suitable uart.
 * proper Pin multiplexing is done here.
 * Please for initial testing we have used Character queue Rx & Tx.
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

#include "custom_MPU_wrappers.h"
//#include "UART-interrupt-driven-command-console.h"

/* Dimensions the buffer into which input characters are placed. */
#define cmdMAX_INPUT_SIZE			50

/* Place holder for calls to ioctl that don't use the value parameter. */
#define cmdPARAMTER_NOT_USED		( ( void * ) 0 )

/* Block times of 50 and 500milliseconds, specified in ticks. */
#define cmd50ms						( ( void * ) ( 50UL / portTICK_RATE_MS ) )
#define cmd500ms					( ( void * ) ( 500UL / portTICK_RATE_MS ) )

/*-----------------------------------------------------------*/


/* GCC specifics. */
#define mainALIGN_TO( x )				__attribute__((aligned(x)))


/* The task functions. */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void foo(void);
/*-----------------------------------------------------------*/
/* Data used by the 'reg test' tasks. -----------------------*/
/*-----------------------------------------------------------*/

/* Define the constants used to allocate the reg test task stacks.  Note that
that stack size is defined in words, not bytes. */
#define configINFOTAINMENT_TASK_STACK_SIZE	128
#define mainREG_TEST_STACK_ALIGNMENT	( configINFOTAINMENT_TASK_STACK_SIZE * sizeof( portSTACK_TYPE ) )

/* Declare the stacks that will be used by the reg test tasks.  The kernel will
automatically create an MPU region for the stack.  The stack alignment must
match its size, so if 128 words are reserved for the stack then it must be
aligned to ( 128 * 4 ) bytes. */
mainALIGN_TO( mainREG_TEST_STACK_ALIGNMENT ) static StackType_t xRegTestStack8[ configINFOTAINMENT_TASK_STACK_SIZE ];




static void prvInfotainmentTask( void);
void pop_registers_from_fault_stack(unsigned int * hardfault_args);

static const int8_t * const pcNewLine = ( int8_t * ) "\r\n";


xTaskHandle xInfotainmentTask = NULL;

static const int8_t * const pcMyMessage = ( int8_t * ) "\033[20;25H Welcome \r\n\r\n";

int global;

static volatile xTaskParameters xRegInfotainmentTaskParameters =
{
		prvInfotainmentTask,						/* pvTaskCode - the function that implements the task. */
		"Display",					/* pcName			*/
		configINFOTAINMENT_TASK_STACK_SIZE,			/* usStackDepth		*/
		NULL,					/* pvParameters - this value is just to test that the parameter is being passed into the task correctly. */
		configINFOTAINMENT_TASK_PRIORITY|portPRIVILEGE_BIT,	/* uxPriority - note that this task is created with privileges to demonstrate one method of passing a queue handle into the task. */
		xRegTestStack8,							/* puxStackBuffer - the array to use as the task stack, as declared above. */
		{										/* xRegions - this task does not use any non-stack data hence all members are zero. */
				/* Base address		Length		Parameters */
				{ 0x00,				0x00,			0x00 },
				{ 0x00,				0x00,			0x00 },
				{ 0x00,				0x00,			0x00 }
		}
};


void vMyInfotainmentTaskStart( void )
{
	xTaskCreateRestricted( &xRegInfotainmentTaskParameters, &xInfotainmentTask );
}

/*-----------------------------------------------------------*/
Peripheral_Descriptor_t xConsoleUART1 = NULL;
static void prvInfotainmentTask( void)
{
	Peripheral_Descriptor_t xConsoleUART = NULL;
	portBASE_TYPE xReturned;
	int8_t * infoMsg;

	/* Open the UART port used for console input.  The second parameter
	(ulFlags) is not used in this case.  The default board rate is set by the
	boardDEFAULT_UART_BAUD parameter.  The baud rate can be changed using a
	FreeRTOS_ioctl() call with the ioctlSET_SPEED command. */
	xConsoleUART = FreeRTOS_open( boardCOMMAND_CONSOLE_UART, ( uint32_t ) cmdPARAMTER_NOT_USED );
	configASSERT( xConsoleUART );
	xConsoleUART1=xConsoleUART;
	portSWITCH_TO_USER_MODE();

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
	FreeRTOS_write( xConsoleUART, "\033[18;18H********************************************************", 55 );
	FreeRTOS_write( xConsoleUART, pcMyMessage, strlen( ( char * ) pcMyMessage ) );
	FreeRTOS_write( xConsoleUART, "\033[25;18H********************************************************", 55 );
	FreeRTOS_write( xConsoleUART, "\033[19;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[20;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[21;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[22;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[23;18H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[24;18H#", 12 );

	FreeRTOS_write( xConsoleUART, "\033[19;64H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[20;64H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[21;64H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[22;64H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[23;64H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[24;64H#", 12 );
	FreeRTOS_write( xConsoleUART, "\033[20;20H#Hello", 16 );

	char buff[20];

	for( ;; )
	{
		//Chip_WWDT_Feed(NULL);
		xReturned = FreeRTOS_read(xConsoleUART, buff, 1);
		configASSERT(xReturned == 1);
		global=0;
		//foo();
	}
}

void foo(void)
{
	volatile int a;
	foo();
}
/*-----------------------------------------------------------*/

