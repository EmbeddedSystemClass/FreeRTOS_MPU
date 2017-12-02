################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Examples/I2C/I2C-Temperature-Read.c \
../Source/Examples/I2C/I2C-myModule.c \
../Source/Examples/I2C/I2C-to-OLED.c \
../Source/Examples/I2C/I2C-to-and-from-EEPROM.c \
../Source/Examples/I2C/font5x7.c \
../Source/Examples/I2C/oled.c 

OBJS += \
./Source/Examples/I2C/I2C-Temperature-Read.o \
./Source/Examples/I2C/I2C-myModule.o \
./Source/Examples/I2C/I2C-to-OLED.o \
./Source/Examples/I2C/I2C-to-and-from-EEPROM.o \
./Source/Examples/I2C/font5x7.o \
./Source/Examples/I2C/oled.o 

C_DEPS += \
./Source/Examples/I2C/I2C-Temperature-Read.d \
./Source/Examples/I2C/I2C-myModule.d \
./Source/Examples/I2C/I2C-to-OLED.d \
./Source/Examples/I2C/I2C-to-and-from-EEPROM.d \
./Source/Examples/I2C/font5x7.d \
./Source/Examples/I2C/oled.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Examples/I2C/%.o: ../Source/Examples/I2C/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/lpc_chip_175x_6x/inc" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Device/LPC17xx/SupportedBoards" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Include" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-CLI" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/include" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source/Examples/Include" -I"/home/desd/clabsarjsau/PROJECT_DATA/Workspace_5_toyota/FreeRTOS-Toyota-Model/Source" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


