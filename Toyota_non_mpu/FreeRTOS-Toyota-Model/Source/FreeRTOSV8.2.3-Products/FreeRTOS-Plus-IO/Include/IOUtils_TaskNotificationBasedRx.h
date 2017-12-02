#ifndef TASK_NOTIFICATION_BASED_RX_H
#define TASK_NOTIFICATION_BASED_RX_H

/* The transfer structure used when a circular buffer is used for
reception. */
typedef struct xTASK_NOTIFICATION_BASED_RX_STATE
{
//	xSemaphoreHandle xNewDataSemaphore;	/* Semaphore used to indicate the presence of new data. */

	TaskHandle_t xTaskToNotify;
	uint8_t *pucBufferStart;			/* The start address of the buffer storage area. */
	uint16_t usBufferLength;			/* The total length of the circular buffer. */
	uint16_t usNextWriteIndex;			/* Index into pucBufferStart to which the next received byte will be written. */
	uint16_t usNextReadIndex;			/* Index into pucBufferStart from which the next byte will be read. */
	uint16_t usErrorState;				/* Currently just set to pdFALSE or pdTRUE if a buffer overrun has not/has occurred respectively. */
	portTickType xBlockTime;			/* The amount of time a task should be held in the Blocked state (not using CPU time) to wait for data to become available when it attempts a read. */
	uint16_t usCharCount;				/* The number of characters currently in the circular buffer.  The difference between the number of characters written to the buffer, and the number read from the buffer. */
} Task_Notification_Based_Rx_State_t;

#define prvTASK_NOTIFICATION_BASED_TX_STATE( pxPeripheralControl ) ( ( Task_Notification_Based_Rx_State_t * ) ( pxPeripheralControl )->pxTxControl->pvTransferState )

/* Transfer type casts from peripheral structs. */
#define prvTASK_NOTIFICATION_BASED_RX_STATE( pxPeripheralControl ) ( ( Task_Notification_Based_Rx_State_t * ) ( pxPeripheralControl )->pxRxControl->pvTransferState )

/* Rx macros. */
#define ioutilsRX_CHARS_INTO_TNBASED_CIRCULAR_BUFFER_FROM_ISR( pxTransferControl, xCondition, xReceiveFunction, ulReceived, xHigherPriorityTaskWoken )	\
{																														\
Task_Notification_Based_Rx_State_t * const pxTaskNotificationBaseRXState = ( Task_Notification_Based_Rx_State_t * ) ( ( pxTransferControl )->pvTransferState );	\
uint16_t *pusNextWrite;																									\
																														\
	pusNextWrite = &( pxTaskNotificationBaseRXState->usNextWriteIndex );														\
																														\
	while( ( xCondition ) )																								\
	{																													\
		/* If there is space in the circular buffer. */																	\
		if( pxTaskNotificationBaseRXState->usCharCount < pxTaskNotificationBaseRXState->usBufferLength )							\
		{																												\
			pxTaskNotificationBaseRXState->pucBufferStart[ *pusNextWrite ] = ( xReceiveFunction );							\
			ulReceived++;																								\
			( pxTaskNotificationBaseRXState->usCharCount )++;																	\
																														\
			/* Move on to the next index, wrapping if necessary. */														\
			( *pusNextWrite )++;																						\
			if( *pusNextWrite == pxTaskNotificationBaseRXState->usBufferLength )												\
			{																											\
				*pusNextWrite = 0U;																						\
			}																											\
		}																												\
		else																											\
		{																												\
			/* An overrun has occurred. */																				\
			pxTaskNotificationBaseRXState->usErrorState = pdTRUE;																\
		}																												\
	}																													\
																														\
	if( ulReceived > 0UL )																								\
	{																													\
		/* Unblock any tasks that might have been waiting for new data to 												\
		arrive. */																										\
		vTaskNotifyGiveFromISR( pxTaskNotificationBaseRXState->xTaskToNotify, &( xHigherPriorityTaskWoken ) );			\
		/*xSemaphoreGiveFromISR( pxCircularBufferRxState->xNewDataSemaphore, &( xHigherPriorityTaskWoken ) );*/	\
	}																													\
}

#define ioutilsRECEIVE_CHARS_FROM_TNBASED_CIRCULAR_BUFFER( pxPeripheralControl, xPeripheralDisable, xPeripheralEnable, pucBuffer, xBytes, xBytesReceived )	\
{																														\
portTickType xTicksToWait;																								\
xTimeOutType xTimeOut;																									\
Task_Notification_Based_Rx_State_t *pxTaskNotificationBaseRXState = prvTASK_NOTIFICATION_BASED_RX_STATE( pxPeripheralControl );					\
																														\
	( xBytesReceived ) = 0U;																							\
	xTicksToWait = pxTaskNotificationBaseRXState->xBlockTime;														\
	vTaskSetTimeOutState( &xTimeOut );																					\
																														\
	/* Are there any more bytes to be received? */																		\
	while( ( xBytesReceived ) < xBytes )																				\
	{																													\
		ulTaskNotifyTake( pdFALSE, xTicksToWait );						\
		{																												\
			( xPeripheralDisable ); 																					\
			( xBytesReceived ) += xIOUtilsReadCharsFromTNBasedCircularBuffer( pxTaskNotificationBaseRXState, &( ( pucBuffer ) [ ( xBytesReceived ) ] ), xBytes - ( xBytesReceived ) ); \
			( xPeripheralEnable );																						\
		}																												\
																														\
		if( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) != pdFALSE )												\
		{																												\
			/* Time out has expired. */																					\
			break;																										\
		}																												\
	}																													\
}

#define ioutilsBLOCKING_SEND_CHARS_TO_TNBASED_CIRCULAR_BUFFER( pxPeripheralControl, pxPeripheralNotBusy, xTransmitFunction, pucBuffer, xTotalBytes, xBytesSent)	\
{																														\
uint16_t *pusNextWrite;																									\
uint16_t *pusNextRead;																									\
portTickType xTicksToWait;																								\
uint8_t ucChar = 'a';																									\
xTimeOutType xTimeOut;																									\
Task_Notification_Based_Rx_State_t *pxTxState = prvTASK_NOTIFICATION_BASED_TX_STATE( pxPeripheralControl );				\
																														\
	xTicksToWait = pxTxState->xBlockTime;																				\
	vTaskSetTimeOutState( &xTimeOut );																					\
	pusNextWrite = &( pxTxState->usNextWriteIndex );																	\
	pusNextRead = &( pxTxState->usNextReadIndex );																	\
																														\
	for( ( xBytesSent ) = 0U; ( xBytesSent ) < ( xTotalBytes );  )										\
	{																													\
		/* If there is space in the circular buffer. */																	\
		if( pxTxState->usCharCount < pxTxState->usBufferLength )														\
		{																												\
			pxTxState->pucBufferStart[ *pusNextWrite ] = ( pucBuffer[ ( xBytesSent ) ] );								\
			/*ulReceived++;*/																							\
			( pxTxState->usCharCount )++;																				\
																														\
			/* Move on to the next index, wrapping if necessary. */														\
			( *pusNextWrite )++;																						\
			if( *pusNextWrite == pxTxState->usBufferLength )															\
			{																											\
				*pusNextWrite = 0U;																						\
			}																											\
			( xBytesSent )++;																							\
			if( ( pxPeripheralNotBusy ) )																					\
			{																												\
																		\
					ucChar = pxTxState->pucBufferStart[ *pusNextRead ];												\
					( pxTxState->usCharCount )--;\
					( *pusNextRead )++;\
					if( *pusNextRead == pxTxState->usBufferLength )															\
					{																											\
						*pusNextRead = 0U;																						\
					}																											\
					( xTransmitFunction );																					\
																															\
			}																												\
		}																												\
		else																											\
		{																												\
			/*if( xBytesSent > 0UL )																								\
			{																													\
				if( ( pxPeripheralNotBusy ) )																					\
				{																												\
																\
						ucChar = pxTxState->pucBufferStart[ *pusNextRead ];												\
						( pxTxState->usCharCount )--;\
						( *pusNextRead )++;\
						if( *pusNextRead == pxTxState->usBufferLength )															\
						{																											\
							*pusNextRead = 0U;																						\
						}																											\
						( xTransmitFunction );																					\
																																\
				}																												\
			}*/																													\
			/*if( pxTxState->usCharCount >= pxTxState->usBufferLength )*/														\
			{																												\
				ulTaskNotifyTake( pdFALSE, pxTxState->xBlockTime );															\
			}/**/																												\
		}																												\
	}																													\
	/*if( xBytesSent > 0UL )																								\
	{																													\
		if( ( pxPeripheralNotBusy ) )																					\
		{																												\
														\
			ucChar = pxTxState->pucBufferStart[ *pusNextRead ];												\
			( pxTxState->usCharCount )--;\
			( *pusNextRead )++;\
			if( *pusNextRead == pxTxState->usBufferLength )															\
			{																											\
				*pusNextRead = 0U;																						\
			}																											\
																														\
			( xTransmitFunction );																					\
																														\
		}																												\
		/*if( pxTxState->usCharCount >= pxTxState->usBufferLength )														\
		{																												\
			ulTaskNotifyTake( pdTRUE, pxTxState->xBlockTime );															\
		}																												\
	}*/																													\
																													\
	/*ulTaskNotifyTake( pdTRUE, pxTxState->xBlockTime );*/															\
																														\
}

#define ioutilsTX_CHARS_FROM_TNBASED_CIRCULAR_BUFFER_FROM_ISR( pxTransferControl, xCondition, xTransmitFunction, xHigherPriorityTaskWoken ) \
{																															\
Task_Notification_Based_Rx_State_t *pxTaskNotificationBaseRXState = ( Task_Notification_Based_Rx_State_t * ) ( ( pxTransferControl )->pvTransferState );	\
uint8_t ucChar;																												\
																															\
	while( ( xCondition ) )																									\
	{																														\
		if( xQueueReceiveFromISR( pxCharQueueState->xQueue, &ucChar, &xHigherPriorityTaskWoken ) == pdTRUE )				\
		{																													\
			( xTransmitFunction );																							\
		}																													\
		else																												\
		{																													\
			break;																											\
		}																													\
	}																														\
}





/* Prototypes of functions that are for internal use only. */
portBASE_TYPE xIOUtilsConfigureTaskNotificationBasedRx( Peripheral_Control_t * const pxPeripheralControl, const uint32_t ulRequest, const portBASE_TYPE xBufferSize );
portBASE_TYPE xIOUtilsReadCharsFromTNBasedCircularBuffer( Task_Notification_Based_Rx_State_t * const pxTNBasedCircularBuffer, uint8_t * const pucBuffer, const size_t xBytesToRead );
portBASE_TYPE xIOUtilsTxCharsFromTNCircularBufferFromISR( Transfer_Control_t *pxTransferStruct, portBASE_TYPE xHigherPriorityTaskWoken  );
//void vIOUtilsSetCircularBufferTimeout( Peripheral_Control_t *pxPeripheralControl, portTickType xMaxWaitTime );
//void vIOUtilsClearRxCircularBuffer( Peripheral_Control_t * const pxPeripheralControl );
#endif




