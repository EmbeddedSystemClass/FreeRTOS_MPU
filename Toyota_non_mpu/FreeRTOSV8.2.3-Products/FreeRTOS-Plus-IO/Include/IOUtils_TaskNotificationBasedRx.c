/* Standard includes. */
#include "string.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Device specific library includes. */
#include "FreeRTOS_DriverInterface.h"
#include "IOUtils_Common.h"

/*-----------------------------------------------------------*/

portBASE_TYPE xIOUtilsConfigureTaskNotificationBasedRx( Peripheral_Control_t * const pxPeripheralControl, const portBASE_TYPE xBufferSize )
{
portBASE_TYPE xReturn = pdFAIL;
Task_Notification_Based_Rx_State_t *pxTaskNotificationBaseRXState;

	/* A peripheral is going to use a Circular_Buffer_Rx_State_t structure to
	control reception. */
	vIOUtilsCreateTransferControlStructure( &( pxPeripheralControl->pxRxControl ) );
	configASSERT( pxPeripheralControl->pxRxControl );
	if( pxPeripheralControl->pxRxControl != NULL )
	{
		/* Create the necessary structure. */
		pxTaskNotificationBaseRXState = pvPortMalloc( sizeof( Task_Notification_Based_Rx_State_t ) );
		if( pxTaskNotificationBaseRXState != NULL )
		{
			pxTaskNotificationBaseRXState->pucBufferStart = pvPortMalloc( xBufferSize );
			if( pxTaskNotificationBaseRXState->pucBufferStart != NULL )
			{
				pxTaskNotificationBaseRXState->xTaskToNotify = xTaskGetCurrentTaskHandle();
				pxTaskNotificationBaseRXState->usBufferLength = ( uint16_t ) xBufferSize;
				pxTaskNotificationBaseRXState->usErrorState = 0U;
				pxTaskNotificationBaseRXState->usNextReadIndex = 0U;
				pxTaskNotificationBaseRXState->usCharCount = 0U;
				pxTaskNotificationBaseRXState->usNextWriteIndex = 0U;
				pxPeripheralControl->pxRxControl->pvTransferState = ( void * ) pxTaskNotificationBaseRXState;
				pxPeripheralControl->pxRxControl->ucType = ioctlUSE_TASK_NOTIFICATION_BASED_RX;
				pxTaskNotificationBaseRXState->xBlockTime = portMAX_DELAY;
				xReturn = pdPASS;
			}
			else
			{
				/* The buffer could not be allocated, so everything
				allocated thus far will have to be freed again and an error
				returned.*/
				vPortFree( pxTaskNotificationBaseRXState );
				pxTaskNotificationBaseRXState = NULL;
			}
		}

		if( pxTaskNotificationBaseRXState == NULL )
		{
			/* The Rx structure, or a member it contains,  could not be created,
			so the Rx control structure (which should point to it) should also
			be deleted. */
			vPortFree( pxPeripheralControl->pxRxControl );
			pxPeripheralControl->pxRxControl = NULL;
		}
	}

	return xReturn;
}
/*-----------------------------------------------------------*/


portBASE_TYPE xIOUtilsReadCharsFromTNBasedCircularBuffer( Task_Notification_Based_Rx_State_t * const pxTNBasedCircularBuffer, uint8_t * const pucBuffer, const size_t xBytesToRead )
{
size_t xBytesAvailable, xBytesReadSoFar = 0U;
portBASE_TYPE xAttempts;
const portBASE_TYPE xMaxAttempts = 2;

	/* Two attempts at reading the required number of bytes are made in	case
	the first read just reads up to the end of the circular buffer before
	wrapping back round to the start - in which case the second	attempt will
	try and read the remaining bytes from the start. */
	for( xAttempts = 0; xAttempts < xMaxAttempts; xAttempts++ )
	{
		if( pxTNBasedCircularBuffer->usCharCount == 0U )
		{
			/* There are no more characters in the buffer, so there is no point
			continuing. */
			break;
		}

		if( pxTNBasedCircularBuffer->usNextReadIndex <= pxTNBasedCircularBuffer->usNextWriteIndex )
		{
			/* How many bytes are available between the read and write
			indexes? */
			xBytesAvailable = ( pxTNBasedCircularBuffer->usNextWriteIndex - pxTNBasedCircularBuffer->usNextReadIndex );
		}
		else
		{
			/* How many bytes are available up to the end of the buffer? */
			xBytesAvailable = ( pxTNBasedCircularBuffer->usBufferLength - pxTNBasedCircularBuffer->usNextReadIndex );
		}

		/* Cap the bytes to read to a maximum of those asked for. */
		if( xBytesAvailable > ( xBytesToRead - xBytesReadSoFar ) )
		{
			xBytesAvailable = ( xBytesToRead - xBytesReadSoFar );
		}

		configASSERT( xBytesAvailable );

		/* Copy the bytes. */
		memcpy( &( pucBuffer[ xBytesReadSoFar ] ), &( pxTNBasedCircularBuffer->pucBufferStart[ pxTNBasedCircularBuffer->usNextReadIndex ] ), xBytesAvailable );

		/* Remember how many bytes have been read for the next iteration of
		this loop. */
		xBytesReadSoFar += xBytesAvailable;

		/* Characters have been removed from the buffer. */
		pxTNBasedCircularBuffer->usCharCount -= xBytesAvailable;
		pxTNBasedCircularBuffer->usNextReadIndex += xBytesAvailable;

		configASSERT( pxTNBasedCircularBuffer->usNextReadIndex <= pxTNBasedCircularBuffer->usBufferLength );

		if( pxTNBasedCircularBuffer->usNextReadIndex == pxTNBasedCircularBuffer->usBufferLength )
		{
			pxTNBasedCircularBuffer->usNextReadIndex = 0U;
		}

		if( xBytesToRead == xBytesReadSoFar )
		{
			/* No more bytes are needed. */
			break;
		}
	}

	return xBytesReadSoFar;
}

