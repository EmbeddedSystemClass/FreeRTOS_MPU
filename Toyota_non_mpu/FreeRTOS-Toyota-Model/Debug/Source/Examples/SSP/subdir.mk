################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Examples/SSP/SPI-writes-to-7-seg-display.c 

OBJS += \
./Source/Examples/SSP/SPI-writes-to-7-seg-display.o 

C_DEPS += \
./Source/Examples/SSP/SPI-writes-to-7-seg-display.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Examples/SSP/%.o: ../Source/Examples/SSP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_LPCOpen_Demo/lpc_chip_175x_6x/inc" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_LPCOpen_Demo/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_LPCOpen_Demo/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Device/LPC17xx/SupportedBoards" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_LPCOpen_Demo/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Include" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_LPCOpen_Demo/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-CLI" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_LPCOpen_Demo/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_LPCOpen_Demo/FreeRTOSV8.2.3-Products/FreeRTOS/Source/include" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_LPCOpen_Demo/FreeRTOS-Plus-Demo-1/Source/Examples/Include" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_LPCOpen_Demo/FreeRTOS-Plus-Demo-1/Source" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


