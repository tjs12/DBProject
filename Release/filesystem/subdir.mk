################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../filesystem/testfilesystem.cpp 

OBJS += \
./filesystem/testfilesystem.o 

CPP_DEPS += \
./filesystem/testfilesystem.d 


# Each subdirectory must supply rules for building sources it contributes
filesystem/%.o: ../filesystem/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


