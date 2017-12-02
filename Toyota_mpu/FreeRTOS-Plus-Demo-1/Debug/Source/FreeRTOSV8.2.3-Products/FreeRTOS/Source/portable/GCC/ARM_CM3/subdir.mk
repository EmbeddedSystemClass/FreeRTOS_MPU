################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c 

OBJS += \
./Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3/port.o 

C_DEPS += \
./Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3/port.d 


# Each subdirectory must supply rules for building sources it contributes
Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3/%.o: ../Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -I"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\lpc_chip_175x_6x\inc" -I"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\FreeRTOS-Plus-Demo-1\Source\FreeRTOSV8.2.3-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\FreeRTOS-Plus-Demo-1\Source\FreeRTOSV8.2.3-Products\FreeRTOS-Plus-IO\Include" -I"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\FreeRTOS-Plus-Demo-1\Source\FreeRTOSV8.2.3-Products\FreeRTOS-Plus-CLI" -I"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\FreeRTOS-Plus-Demo-1\Source\FreeRTOSV8.2.3-Products\FreeRTOS\Source\portable\GCC\ARM_CM3" -I"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\FreeRTOS-Plus-Demo-1\Source\FreeRTOSV8.2.3-Products\FreeRTOS\Source\include" -I"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\FreeRTOS-Plus-Demo-1\Source\Examples\Include" -I"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\FreeRTOS-Plus-Demo-1\Source" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


