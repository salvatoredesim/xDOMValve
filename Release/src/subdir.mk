################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cube_hal_f4.c \
../src/main.c \
../src/stm32f4xx_hal_msp.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/cube_hal_f4.o \
./src/main.o \
./src/stm32f4xx_hal_msp.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/cube_hal_f4.d \
./src/main.d \
./src/stm32f4xx_hal_msp.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DNUCLEO_F401RE -DSTM32F4 -DSTM32F401RETx -DSTM32 -DUSE_HAL_DRIVER -DSTM32F401xE -I"C:/Users/Aeromechs PC/workspace/Motor/inc" -I"C:/Users/Aeromechs PC/workspace/Motor/CMSIS/core" -I"C:/Users/Aeromechs PC/workspace/Motor/CMSIS/device" -I"C:/Users/Aeromechs PC/workspace/Motor/HAL_Driver/Inc/Legacy" -I"C:/Users/Aeromechs PC/workspace/Motor/HAL_Driver/Inc" -I"C:/Users/Aeromechs PC/workspace/Motor/Utilities/STM32F4xx-Nucleo" -I"C:/Users/Aeromechs PC/workspace/Motor/BSP/STM32F4xx-Nucleo" -I"C:/Users/Aeromechs PC/workspace/Motor/BSP/X-NUCLEO-IDB0xA1" -I"C:/Users/Aeromechs PC/workspace/Motor/Middlewares/ST/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"C:/Users/Aeromechs PC/workspace/Motor/Middlewares/ST/STM32_BlueNRG/Interface" -I"C:/Users/Aeromechs PC/workspace/Motor/src/BLE/inc" -I"C:/Users/Aeromechs PC/workspace/Motor/src/Valve/inc" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


