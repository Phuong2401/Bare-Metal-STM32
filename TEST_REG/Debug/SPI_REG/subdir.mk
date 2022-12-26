################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPI_REG/SPI_REG.c 

OBJS += \
./SPI_REG/SPI_REG.o 

C_DEPS += \
./SPI_REG/SPI_REG.d 


# Each subdirectory must supply rules for building sources it contributes
SPI_REG/%.o SPI_REG/%.su: ../SPI_REG/%.c SPI_REG/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"E:/Keil C Project- STM32/TEST/TEST_REG/UART_REG" -I"E:/Keil C Project- STM32/TEST/TEST_REG/GPIO_REG" -I"E:/Keil C Project- STM32/TEST/TEST_REG/SPI_REG" -I"E:/Keil C Project- STM32/TEST/TEST_REG/FLASH_REG" -I"E:/Keil C Project- STM32/TEST/TEST_REG/I2C_reg2" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SPI_REG

clean-SPI_REG:
	-$(RM) ./SPI_REG/SPI_REG.d ./SPI_REG/SPI_REG.o ./SPI_REG/SPI_REG.su

.PHONY: clean-SPI_REG

