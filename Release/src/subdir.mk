################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/config_file.c \
../src/db_mgr.c \
../src/falcon.c \
../src/nw_mgr.c 

OBJS += \
./src/config_file.o \
./src/db_mgr.o \
./src/falcon.o \
./src/nw_mgr.o 

C_DEPS += \
./src/config_file.d \
./src/db_mgr.d \
./src/falcon.d \
./src/nw_mgr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


