
/* Standard includes. */
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+IO includes. */
#include "FreeRTOS_IO.h"
uint32_t temperatureCaptured;   //25.01.17 This is the global variable which will be shared by uart task to display the temp.

#define i2cTEMPERATURE_DATA_SIZE			( 4UL )

#define i2cTemperature_Sensor_SLAVE_ADDRESS	( 0x48UL )

#define i2cTemperature_Sensor_CLOCK 		( ( void * ) 100000UL )

/* Place holder for calls to ioctl that don't use the value parameter. */
#define i2cPARAMETER_NOT_USED	( ( void * ) 0 )

/*-----------------------------------------------------------*/

static uint8_t ucDataBuffer[ i2cTEMPERATURE_DATA_SIZE + 1 ];

/*-----------------------------------------------------------*/

void vI2C_Temperature_Sensor_Test( Peripheral_Descriptor_t xI2CPort )
{
	int32_t lReturned;

	/* The I2C port is opened and configured in the I2C-coordinator.c file.
	The opened handle is passed in to this file - which just uses the I2C
	FreeRTOS+IO driver with whatever configuration it happens to have at that
	time.  Sometimes it	will be operating in polled mode, and other in
	interrupt driven zero copy Tx with interrupt driven circular buffer Rx. */

		/* Set the clock frequency to be correct for the Temperature Sensor. */
		FreeRTOS_ioctl( xI2CPort, ioctlSET_SPEED, i2cTemperature_Sensor_CLOCK );

		/* This is used to set the slave address.*/
		FreeRTOS_I2C_ioctl(xI2CPort, ioctlSET_I2C_SLAVE_ADDRESS,i2cTemperature_Sensor_SLAVE_ADDRESS);

		/* Read from Temperature Sensor(LM75A). */
		lReturned = FreeRTOS_read( xI2CPort, ucDataBuffer, i2cTEMPERATURE_DATA_SIZE );

		configASSERT( lReturned == i2cTEMPERATURE_DATA_SIZE );

		temperatureCaptured = ((ucDataBuffer[0]<<8)|(ucDataBuffer[1]>>5));
}
/*-----------------------------------------------------------*/


