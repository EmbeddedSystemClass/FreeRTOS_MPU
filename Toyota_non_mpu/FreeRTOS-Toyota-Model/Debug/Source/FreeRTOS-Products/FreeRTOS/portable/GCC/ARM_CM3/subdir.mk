################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3/port.c 

OBJS += \
./Source/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3/port.o 

C_DEPS += \
./Source/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3/port.d 


# Each subdirectory must supply rules for building sources it contributes
Source/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3/port.o: /home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3/port.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Device/LPC17xx/SupportedBoards" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Include" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-CLI" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/FreeRTOSV8.2.3-Products/FreeRTOS/Source/include" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/CMSISv2p00_LPC17xx/inc" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/FreeRTOS-Plus-Demo-1/Source/Examples/Include" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/FreeRTOS-Plus-Demo-1/Source" -I"/home/chaitanya/LPCXpresso/Internship_20_04_16/FreeRTOSV8.2.3_Plus_Demo/lpc17xx.cmsis.driver.library/Include" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


