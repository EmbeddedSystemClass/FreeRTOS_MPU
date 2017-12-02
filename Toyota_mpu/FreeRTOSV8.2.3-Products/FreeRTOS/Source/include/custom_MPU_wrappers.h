/*
 * custom_MPU_wrappers.h
 *
 *  Created on: 26-Jan-2017
 *      Author: swapnil
 */

#ifndef FREERTOSV8_2_3_PRODUCTS_FREERTOS_SOURCE_INCLUDE_CUSTOM_MPU_WRAPPERS_H_
#define FREERTOSV8_2_3_PRODUCTS_FREERTOS_SOURCE_INCLUDE_CUSTOM_MPU_WRAPPERS_H_


//#ifdef USE_MY_WRAPPERS

#define FreeRTOS_open					MPU_FreeRTOS_open
#define FreeRTOS_ioctl					MPU_FreeRTOS_ioctl
#define	FreeRTOS_read					MPU_FreeRTOS_read
#define	FreeRTOS_write					MPU_FreeRTOS_write
#define	FreeRTOS_I2C_ioctl				MPU_FreeRTOS_I2C_ioctl



//#endif

#endif /* FREERTOSV8_2_3_PRODUCTS_FREERTOS_SOURCE_INCLUDE_CUSTOM_MPU_WRAPPERS_H_ */
