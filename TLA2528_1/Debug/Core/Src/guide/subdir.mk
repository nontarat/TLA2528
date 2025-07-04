################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/guide/tla2528.c 

C_DEPS += \
./Core/Src/guide/tla2528.d 

OBJS += \
./Core/Src/guide/tla2528.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/guide/%.o Core/Src/guide/%.su Core/Src/guide/%.cyclo: ../Core/Src/guide/%.c Core/Src/guide/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-guide

clean-Core-2f-Src-2f-guide:
	-$(RM) ./Core/Src/guide/tla2528.cyclo ./Core/Src/guide/tla2528.d ./Core/Src/guide/tla2528.o ./Core/Src/guide/tla2528.su

.PHONY: clean-Core-2f-Src-2f-guide

