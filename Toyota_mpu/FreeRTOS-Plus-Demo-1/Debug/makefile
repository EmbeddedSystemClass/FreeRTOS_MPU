################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Source/Infotainment/subdir.mk
-include Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/portable/GCC/ARM_CM3/subdir.mk
-include Source/FreeRTOSV8.2.3-Products/FreeRTOS/Source/subdir.mk
-include Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Include/subdir.mk
-include Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Device/LPC17xx/subdir.mk
-include Source/FreeRTOSV8.2.3-Products/FreeRTOS-Plus-IO/Common/subdir.mk
-include Source/Fault_handling/subdir.mk
-include Source/Examples/Ultrasonic_GPIO/subdir.mk
-include Source/Examples/UART/subdir.mk
-include Source/Examples/I2C/subdir.mk
-include Source/ETCS/Control_Task/subdir.mk
-include Source/ETCS/Brake_Task/subdir.mk
-include Source/ETCS/Acc_Task/subdir.mk
-include Source/Display/subdir.mk
-include Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: FreeRTOS-Plus-Demo-1.axf

# Tool invocations
FreeRTOS-Plus-Demo-1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\lpc_chip_175x_6x\Debug" -L"C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\lpc_board_nxp_lpcxpresso_1769\Debug" -Xlinker -Map=FreeRTOS-Plus-Demo-1.map -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "My_Examples_Debug.ld" -L "C:\Users\Kalidas Tate\Documents\LPCXpresso_8.2.2_650\workspaceTEST\FreeRTOS-Plus-Demo-1\Source" -o "FreeRTOS-Plus-Demo-1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) FreeRTOS-Plus-Demo-1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size FreeRTOS-Plus-Demo-1.axf; # arm-none-eabi-objcopy -O binary FreeRTOS-Plus-Demo-1.axf FreeRTOS-Plus-Demo-1.bin ; checksum -p LPC1769 -d FreeRTOS-Plus-Demo-1.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
