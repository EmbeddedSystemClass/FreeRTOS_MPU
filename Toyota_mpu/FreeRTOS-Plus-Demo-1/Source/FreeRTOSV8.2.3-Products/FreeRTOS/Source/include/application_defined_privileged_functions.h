/*
 * application_defined_privileged_functions.h
 *
 *  Created on: 26-Jan-2017
 *      Author: swapnil
 */

#ifndef FREERTOSV8_2_3_PRODUCTS_FREERTOS_SOURCE_INCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS_H_
#define FREERTOSV8_2_3_PRODUCTS_FREERTOS_SOURCE_INCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS_H_


#include "FreeRTOS_DriverInterface.h"
//#include "mpu_wrappers.h"




//////////////////////////////////////////////////////////////////////////////////////
					/*	My System call routines*/

Peripheral_Descriptor_t MPU_FreeRTOS_open( const int8_t *pcPath, const uint32_t ulFlags )
{
	Peripheral_Descriptor_t xReturn;
	BaseType_t xRunningPrivileged = prvRaisePrivilege();

	xReturn=FreeRTOS_open(pcPath,ulFlags);

	portRESET_PRIVILEGE( xRunningPrivileged );

	return xReturn;
}

portBASE_TYPE MPU_FreeRTOS_ioctl( Peripheral_Descriptor_t const xPeripheral, uint32_t ulRequest, void *pvValue )
{
	portBASE_TYPE xReturn;
	BaseType_t xRunningPrivileged = prvRaisePrivilege();

	xReturn=FreeRTOS_ioctl(xPeripheral, ulRequest, pvValue);

	portRESET_PRIVILEGE( xRunningPrivileged );
	return xReturn;
}

size_t MPU_FreeRTOS_read( Peripheral_Descriptor_t const pxPeripheral, void * const pvBuffer, const size_t xBytes )
{
	size_t xReturn;
	BaseType_t xRunningPrivileged = prvRaisePrivilege();

	xReturn=( ( Peripheral_Control_t * ) pxPeripheral )->read( ( ( Peripheral_Control_t * ) pxPeripheral ), ( pvBuffer ), ( xBytes ) );

	portRESET_PRIVILEGE( xRunningPrivileged );

	return xReturn;
}

size_t MPU_FreeRTOS_write( Peripheral_Descriptor_t const pxPeripheral, void * const pvBuffer, const size_t xBytes )
{
	size_t xReturn;
	BaseType_t xRunningPrivileged = prvRaisePrivilege();

	xReturn=( ( Peripheral_Control_t * ) pxPeripheral )->write( ( ( Peripheral_Control_t * ) pxPeripheral ), ( pvBuffer ), ( xBytes ) );

	portRESET_PRIVILEGE( xRunningPrivileged );

	return xReturn;
}




portBASE_TYPE MPU_FreeRTOS_I2C_ioctl( Peripheral_Descriptor_t const pxPeripheral, uint32_t ulRequest, void *pvValue )
{
	portBASE_TYPE xReturn;
	BaseType_t xRunningPrivileged = prvRaisePrivilege();

	xReturn=FreeRTOS_I2C_ioctl(pxPeripheral, ulRequest, pvValue);

	portRESET_PRIVILEGE( xRunningPrivileged );
	return xReturn;
}






#endif /* FREERTOSV8_2_3_PRODUCTS_FREERTOS_SOURCE_INCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS_H_ */
