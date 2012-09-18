################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/jimmy/Documents/JFramework/JFramework/Source/AutoCrit.cpp \
/home/jimmy/Documents/JFramework/JFramework/Source/Common.cpp \
/home/jimmy/Documents/JFramework/JFramework/Source/GameApp.cpp \
/home/jimmy/Documents/JFramework/JFramework/Source/GameObject.cpp \
/home/jimmy/Documents/JFramework/JFramework/Source/LUAObject.cpp \
/home/jimmy/Documents/JFramework/JFramework/Source/PhysicsObject.cpp \
/home/jimmy/Documents/JFramework/JFramework/Source/main.cpp 

OBJS += \
./Source/AutoCrit.o \
./Source/Common.o \
./Source/GameApp.o \
./Source/GameObject.o \
./Source/LUAObject.o \
./Source/PhysicsObject.o \
./Source/main.o 

CPP_DEPS += \
./Source/AutoCrit.d \
./Source/Common.d \
./Source/GameApp.d \
./Source/GameObject.d \
./Source/LUAObject.d \
./Source/PhysicsObject.d \
./Source/main.d 


# Each subdirectory must supply rules for building sources it contributes
Source/AutoCrit.o: /home/jimmy/Documents/JFramework/JFramework/Source/AutoCrit.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/Common.o: /home/jimmy/Documents/JFramework/JFramework/Source/Common.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/GameApp.o: /home/jimmy/Documents/JFramework/JFramework/Source/GameApp.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/GameObject.o: /home/jimmy/Documents/JFramework/JFramework/Source/GameObject.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/LUAObject.o: /home/jimmy/Documents/JFramework/JFramework/Source/LUAObject.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/PhysicsObject.o: /home/jimmy/Documents/JFramework/JFramework/Source/PhysicsObject.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/main.o: /home/jimmy/Documents/JFramework/JFramework/Source/main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


