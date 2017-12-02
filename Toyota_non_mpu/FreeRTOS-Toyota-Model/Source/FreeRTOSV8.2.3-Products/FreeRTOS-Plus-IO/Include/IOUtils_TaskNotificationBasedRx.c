/* Standard includes. */
#include "string.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "FreeRTOS_IO.h"

/* Device specific library includes. */
#include "FreeRTOS_DriverInterface.h"
#include "IOUtils_Common.h"

#define UART_FIFOLVL_TXFIFOLVL(n)	((uint32_t)((n>>8)&0x0F))	/**< Reflects the current level of the UART transmitter FIFO */

/*-----------------------------------------------------------*/

portBASE_TYPE xIOUtilsConfigureTaskNotificationBasedRx( Peripheral_Control_t * const pxPeripheralControl, const uint32_t ulRequest, const portBASE_TYPE xBufferSize )
{
portBASE_TYPE xReturn = pdFAIL;
Task_Notification_Based_Rx_State_t *pxTaskNotificationBaseRXState;
Transfer_Control_t **ppxTransferControl;

	if( ulRequest == ioctlUSE_TASK_NOTIFICATION_BASED_TX )
	{
		ppxTransferControl = &( pxPeripheralControl->pxTxControl );
	}
	else
	{
		ppxTransferControl = &( pxPeripheralControl->pxRxControl );
	}

	/* A peripheral is going to use a Task_Notification_Based_Rx_State_t structure to
	control reception. */
	vIOUtilsCreateTransferControlStructure( ppxTransferControl );
	configASSERT( *ppxTransferControl );
	if( *ppxTransferControl != NULL )
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
				( *ppxTransferControl )->pvTransferState = ( void * ) pxTaskNotificationBaseRXState;
				( *ppxTransferControl )->ucType = ulRequest;
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
			vPortFree( *ppxTransferControl );
			*ppxTransferControl = NULL;
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

portBASE_TYPE xIOUtilsTxCharsFromTNCircularBufferFromISR( Transfer_Control_t *pxTransferStruct, portBASE_TYPE xHigherPriorityTaskWoken  )
{
	Task_Notification_Based_Rx_State_t *pxTaskNotificationBaseRXState = ( Task_Notification_Based_Rx_State_t * ) ( ( pxTransferStruct )->pvTransferState );
	size_t xBytesAvailable, xBytesSendSoFar = 0U;
//	portBASE_TYPE xAttempts;
//	const portBASE_TYPE xMaxAttempts = 2;
	size_t i;
	size_t xAvailableFifo;

		/* Two attempts at reading the required number of bytes are made in	case
		the first read just reads up to the end of the circular buffer before
		wrapping back round to the start - in which case the second	attempt will
		try and read the remaining bytes from the start. */
		while(( UART_FIFOLVL_TXFIFOLVL( LPC_UART3->FIFOLVL ) != ( UART_TX_FIFO_SIZE - 1 ) ))
//		while( pxTaskNotificationBaseRXState->usCharCount )
		{
			//xAvailableFifo = (UART_TX_FIFO_SIZE - 1) - UART_FIFOLVL_TXFIFOLVL( LPC_UART3->FIFOLVL );

			if( pxTaskNotificationBaseRXState->usCharCount == 0)
			{
				/* There are no more characters in the buffer, so there is no point
				continuing. */
				//vTaskNotifyGiveFromISR( pxTaskNotificationBaseRXState->xTaskToNotify, &xHigherPriorityTaskWoken );
				break;
			}
			else
			{

				LPC_UART3->THR = pxTaskNotificationBaseRXState->pucBufferStart[ pxTaskNotificationBaseRXState->usNextReadIndex ];
				( pxTaskNotificationBaseRXState->usCharCount )--;
				( pxTaskNotificationBaseRXState->usNextReadIndex )++;
				if( pxTaskNotificationBaseRXState->usNextReadIndex == pxTaskNotificationBaseRXState->usBufferLength )
				{
					pxTaskNotificationBaseRXState->usNextReadIndex = 0U;
				}
				//vTaskNotifyGiveFromISR( pxTaskNotificationBaseRXState->xTaskToNotify, &xHigherPriorityTaskWoken );
			}
			//vTaskNotifyGiveFromISR( pxTaskNotificationBaseRXState->xTaskToNotify, &xHigherPriorityTaskWoken );

		}
		//vTaskNotifyGiveFromISR( pxTaskNotificationBaseRXState->xTaskToNotify, &xHigherPriorityTaskWoken );
		return xBytesSendSoFar;

}
