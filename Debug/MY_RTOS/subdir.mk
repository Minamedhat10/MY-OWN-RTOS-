################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MY_RTOS/CortexMX_OS_porting.c \
../MY_RTOS/MYRTOS._FIFO.c \
../MY_RTOS/scheduler.c 

OBJS += \
./MY_RTOS/CortexMX_OS_porting.o \
./MY_RTOS/MYRTOS._FIFO.o \
./MY_RTOS/scheduler.o 

C_DEPS += \
./MY_RTOS/CortexMX_OS_porting.d \
./MY_RTOS/MYRTOS._FIFO.d \
./MY_RTOS/scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
MY_RTOS/CortexMX_OS_porting.o: ../MY_RTOS/CortexMX_OS_porting.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"G:/KS_STM32xx/MY_RTOS/inc" -I"G:/KS_STM32xx/CMSIS_v5" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MY_RTOS/CortexMX_OS_porting.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
MY_RTOS/MYRTOS._FIFO.o: ../MY_RTOS/MYRTOS._FIFO.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"G:/KS_STM32xx/MY_RTOS/inc" -I"G:/KS_STM32xx/CMSIS_v5" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MY_RTOS/MYRTOS._FIFO.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
MY_RTOS/scheduler.o: ../MY_RTOS/scheduler.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"G:/KS_STM32xx/MY_RTOS/inc" -I"G:/KS_STM32xx/CMSIS_v5" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"MY_RTOS/scheduler.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

