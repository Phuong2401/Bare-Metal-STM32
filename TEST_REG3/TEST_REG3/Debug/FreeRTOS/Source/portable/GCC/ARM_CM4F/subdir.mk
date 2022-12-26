################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/GCC/ARM_CM4F/%.o FreeRTOS/Source/portable/GCC/ARM_CM4F/%.su: ../FreeRTOS/Source/portable/GCC/ARM_CM4F/%.c FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Keil C Project- STM32/TEST_BOARD/TEST_REG3/TEST_REG3/I2C_reg" -I"E:/Keil C Project- STM32/TEST_BOARD/TEST_REG3/TEST_REG3/FreeRTOS" -I"E:/Keil C Project- STM32/TEST_BOARD/TEST_REG3/TEST_REG3/FreeRTOS/Source/include" -I"E:/Keil C Project- STM32/TEST_BOARD/TEST_REG3/TEST_REG3/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"E:/Keil C Project- STM32/TEST_BOARD/TEST_REG3/TEST_REG3/FreeRTOS/Source/portable/MemMang" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F

clean-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F:
	-$(RM) ./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.d ./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o ./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.su

.PHONY: clean-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F

