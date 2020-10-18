################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/asip/asip.c \
../src/asip/call.c \
../src/asip/help.c \
../src/asip/listen_sip.c \
../src/asip/login.c 

OBJS += \
./src/asip/asip.o \
./src/asip/call.o \
./src/asip/help.o \
./src/asip/listen_sip.o \
./src/asip/login.o 

C_DEPS += \
./src/asip/asip.d \
./src/asip/call.d \
./src/asip/help.d \
./src/asip/listen_sip.d \
./src/asip/login.d 


# Each subdirectory must supply rules for building sources it contributes
src/asip/%.o: ../src/asip/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DHAVE_INTTYPES_H -I../include -I/usr/local/include/re -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


