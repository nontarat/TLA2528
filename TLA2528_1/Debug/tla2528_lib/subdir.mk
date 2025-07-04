################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tla2528_lib/TLA2528.c 

OBJS += \
./tla2528_lib/TLA2528.o 

C_DEPS += \
./tla2528_lib/TLA2528.d 


# Each subdirectory must supply rules for building sources it contributes
tla2528_lib/%.o tla2528_lib/%.su tla2528_lib/%.cyclo: ../tla2528_lib/%.c tla2528_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/TLA2528/TLA2528_1/tla2528_lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tla2528_lib

clean-tla2528_lib:
	-$(RM) ./tla2528_lib/TLA2528.cyclo ./tla2528_lib/TLA2528.d ./tla2528_lib/TLA2528.o ./tla2528_lib/TLA2528.su

.PHONY: clean-tla2528_lib

