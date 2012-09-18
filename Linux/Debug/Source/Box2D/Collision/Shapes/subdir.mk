################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/jimmy/Documents/JFramework/JFramework/Source/Box2D/Collision/Shapes/b2ChainShape.cpp \
/home/jimmy/Documents/JFramework/JFramework/Source/Box2D/Collision/Shapes/b2CircleShape.cpp \
/home/jimmy/Documents/JFramework/JFramework/Source/Box2D/Collision/Shapes/b2EdgeShape.cpp \
/home/jimmy/Documents/JFramework/JFramework/Source/Box2D/Collision/Shapes/b2PolygonShape.cpp 

OBJS += \
./Source/Box2D/Collision/Shapes/b2ChainShape.o \
./Source/Box2D/Collision/Shapes/b2CircleShape.o \
./Source/Box2D/Collision/Shapes/b2EdgeShape.o \
./Source/Box2D/Collision/Shapes/b2PolygonShape.o 

CPP_DEPS += \
./Source/Box2D/Collision/Shapes/b2ChainShape.d \
./Source/Box2D/Collision/Shapes/b2CircleShape.d \
./Source/Box2D/Collision/Shapes/b2EdgeShape.d \
./Source/Box2D/Collision/Shapes/b2PolygonShape.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Box2D/Collision/Shapes/b2ChainShape.o: /home/jimmy/Documents/JFramework/JFramework/Source/Box2D/Collision/Shapes/b2ChainShape.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/Box2D/Collision/Shapes/b2CircleShape.o: /home/jimmy/Documents/JFramework/JFramework/Source/Box2D/Collision/Shapes/b2CircleShape.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/Box2D/Collision/Shapes/b2EdgeShape.o: /home/jimmy/Documents/JFramework/JFramework/Source/Box2D/Collision/Shapes/b2EdgeShape.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/Box2D/Collision/Shapes/b2PolygonShape.o: /home/jimmy/Documents/JFramework/JFramework/Source/Box2D/Collision/Shapes/b2PolygonShape.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../Source -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


