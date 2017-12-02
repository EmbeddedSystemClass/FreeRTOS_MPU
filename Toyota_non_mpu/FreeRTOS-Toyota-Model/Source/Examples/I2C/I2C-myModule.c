/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTO+IO includes. */
#include "FreeRTOS_IO.h"
/*-----------------------------------------------------------*/

/* Place holder for calls to ioctl that don't use the value parameter. */
#define i2cPARAMETER_NOT_USED			( ( void * ) 0 )

/* The size, in bytes, of the circular buffer used when the I2C port is using
circular buffer Rx mode. */
#define i2cCIRCULAR_BUFFER_SIZE			( ( void * ) 50 )

/* The maximum amount of time to wait to receive the requested number of bytes
when using zero copy receive mode. */
#define i2c200MS_TIMEOUT				( ( void * ) ( 200UL / portTICK_RATE_MS ) )
/*-----------------------------------------------------------*/

#define i2cTEMPERATURE_DATA_SIZE			( 4UL )

#define i2cTemperature_Sensor_SLAVE_ADDRESS	( 0x48UL )

#define i2cTemperature_Sensor_CLOCK 		( ( void * ) 100000UL )

/* Place holder for calls to ioctl that don't use the value parameter. */
#define i2cPARAMETER_NOT_USED	( ( void * ) 0 )


/*-----------------------------------------------------------*/
//extern uint32_t temperatureCaptured;   //25.01.17 This is the global variable which will be shared by uart task to display the temp.
extern QueueHandle_t xGlobalQueue;
typedef struct
{
	char ucMessageID;
	unsigned int ucValue;
} xQueueData;

static uint8_t ucDataBuffer[ i2cTEMPERATURE_DATA_SIZE + 1 ];
/*
 * The task that uses the I2C to communicate with the OLED.
 */
static void prvMyI2CTask( void *pvParameters );

/*-----------------------------------------------------------*/

void vMyI2CTaskStart( void )
{
	/* Just create the I2C task, then return.  The I2C task communicates with
	the Temperature Sensor. */
	xTaskCreate( 	prvMyI2CTask,								/* The task that uses the I2C peripheral. */
			( const int8_t * const ) "I2C0",	/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
			configI2C_TASK_STACK_SIZE,				/* The size of the stack allocated to the task. */
			NULL,									/* The parameter is not used, so NULL is passed. */
			configI2C_TASK_PRIORITY,				/* The priority allocated to the task. */
			NULL );									/* A handle to the task being created is not required, so just pass in NULL. */
}
/*-----------------------------------------------------------*/


static void prvMyI2CTask( void *pvParameters )
{
	Peripheral_Descriptor_t xI2CPort;
	const uint32_t ulMaxDelay = 500UL / portTICK_RATE_MS;
	int32_t lReturned;

	uint32_t temperatureCaptured;
	xQueueData temperature_queue_data;

	( void ) pvParameters;

	/*copying the global queue in the stack area of this task in privileged mode*/
	xQueueHandle xLocalQueue=xGlobalQueue;


	/* Open the I2C port used for reading from Temperature Sensor.  The
	second parameter (ulFlags) is not used in this case.  The port is opened in
	polling mode.  It is changed to interrupt driven mode later in this
	function. */
	xI2CPort = FreeRTOS_open( boardOLED_I2C_PORT, ( uint32_t ) i2cPARAMETER_NOT_USED );

	configASSERT( xI2CPort );

	/* The I2C port is opened and configured in the I2C-coordinator.c file.
		The opened handle is passed in to this file - which just uses the I2C
		FreeRTOS+IO driver with whatever configuration it happens to have at that
		time.  Sometimes it	will be operating in polled mode, and other in
		interrupt driven zero copy Tx with interrupt driven circular buffer Rx. */

	/* Set the clock frequency to be correct for the Temperature Sensor. */
	FreeRTOS_ioctl( xI2CPort, ioctlSET_SPEED, i2cTemperature_Sensor_CLOCK );

	/* This is used to set the slave address.*/
	FreeRTOS_I2C_ioctl(xI2CPort, ioctlSET_I2C_SLAVE_ADDRESS,i2cTemperature_Sensor_SLAVE_ADDRESS);



	for( ;; )
	{
		/* Read from Temperature Sensor(LM75A). */
		lReturned = FreeRTOS_read( xI2CPort, ucDataBuffer, i2cTEMPERATURE_DATA_SIZE );

		configASSERT( lReturned == i2cTEMPERATURE_DATA_SIZE );

		//temperatureCaptured = ((ucDataBuffer[0]<<3)|(ucDataBuffer[1]>>5));

		temperatureCaptured = ((ucDataBuffer[0]<<3)|(ucDataBuffer[1]>>5));
		temperature_queue_data.ucMessageID=I2C0;
		temperature_queue_data.ucValue=((uint32_t)(temperatureCaptured*0.125));


		/* Send the message to the queue, waiting for 10 ticks for space to become
				available if the queue is already full. */
		if( xQueueSendToBack( xLocalQueue, &temperature_queue_data, 10 ) != pdPASS )
		{
			/* Data could not be sent to the queue even after waiting 10 ticks. */

			//do something important here
		}

		vTaskDelay(25);
	}
}
/*-----------------------------------------------------------*/
