################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../I2C_reg2/I2C_reg2.c 

OBJS += \
./I2C_reg2/I2C_reg2.o 

C_DEPS += \
./I2C_reg2/I2C_reg2.d 


# Each subdirectory must supply rules for building sources it contributes
I2C_reg2/%.o I2C_reg2/%.su: ../I2C_reg2/%.c I2C_reg2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Keil C Project- STM32/TEST/TEST_REG/UART_REG" -I"E:/Keil C Project- STM32/TEST/TEST_REG/GPIO_REG" -I"E:/Keil C Project- STM32/TEST/TEST_REG/SPI_REG" -I"E:/Keil C Project- STM32/TEST/TEST_REG/FLASH_REG" -I"E:/Keil C Project- STM32/TEST/TEST_REG/I2C_reg2" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-I2C_reg2

clean-I2C_reg2:
	-$(RM) ./I2C_reg2/I2C_reg2.d ./I2C_reg2/I2C_reg2.o ./I2C_reg2/I2C_reg2.su

.PHONY: clean-I2C_reg2

