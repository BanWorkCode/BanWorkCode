################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_iwdg.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rtc.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rtc_ex.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c \
../Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c 

OBJS += \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_iwdg.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rtc.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rtc_ex.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.o \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.o 

C_DEPS += \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_iwdg.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rtc.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rtc_ex.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.d \
./Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F3xx_HAL_Driver/Src/%.o: ../Drivers/STM32F3xx_HAL_Driver/Src/%.c Drivers/STM32F3xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"Z:/Portfolio/Controller_for_fans/Controller_for_fans_F303_Cube/ssd1306" -I"Z:/Portfolio/Controller_for_fans/Controller_for_fans_F303_Cube/Display" -I"Z:/Portfolio/Controller_for_fans/Controller_for_fans_F303_Cube/Fans" -I"Z:/Portfolio/Controller_for_fans/Controller_for_fans_F303_Cube/fillMenu" -I"Z:/Portfolio/Controller_for_fans/Controller_for_fans_F303_Cube/Keyboard" -I"Z:/Portfolio/Controller_for_fans/Controller_for_fans_F303_Cube/StoringSettings" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

