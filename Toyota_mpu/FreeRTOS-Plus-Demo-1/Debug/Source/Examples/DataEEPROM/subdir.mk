################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Examples/DataEEPROM/DataEEPROM.c 

OBJS += \
./Source/Examples/DataEEPROM/DataEEPROM.o 

C_DEPS += \
./Source/Examples/DataEEPROM/DataEEPROM.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Examples/DataEEPROM/%.o: ../Source/Examples/DataEEPROM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -I"/home/swapnil/Desktop/TestWorkspace/lpc_chip_175x_6x/inc" -I"/home/swapnil/Desktop/TestWorkspace/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/swapnil/Desktop/TestWorkspace/FreeRTOS-Plus-Demo-1/Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Device/LPC17xx/SupportedBoards" -I"/home/swapnil/Desktop/TestWorkspace/FreeRTOS-Plus-Demo-1/Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Include" -I"/home/swapnil/Desktop/TestWorkspace/FreeRTOS-Plus-Demo-1/Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-CLI" -I"/home/swapnil/Desktop/TestWorkspace/FreeRTOS-Plus-Demo-1/Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/home/swapnil/Desktop/TestWorkspace/FreeRTOS-Plus-Demo-1/Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/include" -I"/home/swapnil/Desktop/TestWorkspace/FreeRTOS-Plus-Demo-1/Source/Examples/Include" -I"/home/swapnil/Desktop/TestWorkspace/FreeRTOS-Plus-Demo-1/Source" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


