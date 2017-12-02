################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/ETCS/Control_Task/Control_Task.c 

OBJS += \
./Source/ETCS/Control_Task/Control_Task.o 

C_DEPS += \
./Source/ETCS/Control_Task/Control_Task.d 


# Each subdirectory must supply rules for building sources it contributes
Source/ETCS/Control_Task/%.o: ../Source/ETCS/Control_Task/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/lpc_chip_175x_6x/inc" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Device/LPC17xx/SupportedBoards" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Include" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-CLI" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/include" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/Examples/Include" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


