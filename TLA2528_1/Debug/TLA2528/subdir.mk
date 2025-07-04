################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TLA2528/TLA2528.c 

OBJS += \
./TLA2528/TLA2528.o 

C_DEPS += \
./TLA2528/TLA2528.d 


# Each subdirectory must supply rules for building sources it contributes
TLA2528/%.o TLA2528/%.su TLA2528/%.cyclo: ../TLA2528/%.c TLA2528/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/TLA2528/TLA2528_1/TLA2528" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TLA2528

clean-TLA2528:
	-$(RM) ./TLA2528/TLA2528.cyclo ./TLA2528/TLA2528.d ./TLA2528/TLA2528.o ./TLA2528/TLA2528.su

.PHONY: clean-TLA2528

