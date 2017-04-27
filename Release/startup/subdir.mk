################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f401xe.s 

OBJS += \
./startup/startup_stm32f401xe.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/Aeromechs PC/workspace/Motor/inc" -I"C:/Users/Aeromechs PC/workspace/Motor/CMSIS/core" -I"C:/Users/Aeromechs PC/workspace/Motor/CMSIS/device" -I"C:/Users/Aeromechs PC/workspace/Motor/HAL_Driver/Inc/Legacy" -I"C:/Users/Aeromechs PC/workspace/Motor/HAL_Driver/Inc" -I"C:/Users/Aeromechs PC/workspace/Motor/Utilities/STM32F4xx-Nucleo" -I"C:/Users/Aeromechs PC/workspace/Motor/BSP/STM32F4xx-Nucleo" -I"C:/Users/Aeromechs PC/workspace/Motor/BSP/X-NUCLEO-IDB0xA1" -I"C:/Users/Aeromechs PC/workspace/Motor/Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"C:/Users/Aeromechs PC/workspace/Motor/Middlewares/ST/STM32_BlueNRG/Interface" -I"C:/Users/Aeromechs PC/workspace/Motor/src/BLE/inc" -I"C:/Users/Aeromechs PC/workspace/Motor/src/Valve/inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


