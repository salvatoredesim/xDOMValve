################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BLE/src/SPBTLE_RF.c \
../src/BLE/src/bluenrg_interface.c \
../src/BLE/src/sensor_service.c 

OBJS += \
./src/BLE/src/SPBTLE_RF.o \
./src/BLE/src/bluenrg_interface.o \
./src/BLE/src/sensor_service.o 

C_DEPS += \
./src/BLE/src/SPBTLE_RF.d \
./src/BLE/src/bluenrg_interface.d \
./src/BLE/src/sensor_service.d 


# Each subdirectory must supply rules for building sources it contributes
src/BLE/src/%.o: ../src/BLE/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DNUCLEO_F401RE -DSTM32F4 -DSTM32F401RETx -DSTM32 -DUSE_HAL_DRIVER -DSTM32F401xE -I"C:/Users/Aeromechs PC/workspace/Motor/inc" -I"C:/Users/Aeromechs PC/workspace/Motor/CMSIS/core" -I"C:/Users/Aeromechs PC/workspace/Motor/CMSIS/device" -I"C:/Users/Aeromechs PC/workspace/Motor/HAL_Driver/Inc/Legacy" -I"C:/Users/Aeromechs PC/workspace/Motor/HAL_Driver/Inc" -I"C:/Users/Aeromechs PC/workspace/Motor/Utilities/STM32F4xx-Nucleo" -I"C:/Users/Aeromechs PC/workspace/Motor/BSP/STM32F4xx-Nucleo" -I"C:/Users/Aeromechs PC/workspace/Motor/BSP/X-NUCLEO-IDB0xA1" -I"C:/Users/Aeromechs PC/workspace/Motor/Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"C:/Users/Aeromechs PC/workspace/Motor/Middlewares/ST/STM32_BlueNRG/Interface" -I"C:/Users/Aeromechs PC/workspace/Motor/src/BLE/inc" -I"C:/Users/Aeromechs PC/workspace/Motor/src/Valve/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


