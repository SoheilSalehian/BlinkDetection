################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../test/AllTests.cpp \
../test/algorithmTest.cpp 

OBJS += \
./test/AllTests.o \
./test/algorithmTest.o 

CPP_DEPS += \
./test/AllTests.d \
./test/algorithmTest.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/local/include/opencv -I/usr/local/lib -I/home/soheil/gmock/gmock-1.7.0/include -I/home/soheil/gmock/gmock-1.7.0/gtest/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


