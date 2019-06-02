################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/argedit.cc \
../src/argloader.cc \
../src/argraph.cc \
../src/error.cc \
../src/gene.cc \
../src/gene_mesh.cc \
../src/match.cc \
../src/sd_state.cc \
../src/sortnodes.cc \
../src/ull_state.cc \
../src/ull_sub_state.cc \
../src/vf2_mono_state.cc \
../src/vf2_state.cc \
../src/vf2_sub_state.cc \
../src/vf_mono_state.cc \
../src/vf_state.cc \
../src/vf_sub_state.cc \
../src/xsubgraph.cc 

CPP_SRCS += \
../src/Matching.cpp 

CC_DEPS += \
./src/argedit.d \
./src/argloader.d \
./src/argraph.d \
./src/error.d \
./src/gene.d \
./src/gene_mesh.d \
./src/match.d \
./src/sd_state.d \
./src/sortnodes.d \
./src/ull_state.d \
./src/ull_sub_state.d \
./src/vf2_mono_state.d \
./src/vf2_state.d \
./src/vf2_sub_state.d \
./src/vf_mono_state.d \
./src/vf_state.d \
./src/vf_sub_state.d \
./src/xsubgraph.d 

OBJS += \
./src/Matching.o \
./src/argedit.o \
./src/argloader.o \
./src/argraph.o \
./src/error.o \
./src/gene.o \
./src/gene_mesh.o \
./src/match.o \
./src/sd_state.o \
./src/sortnodes.o \
./src/ull_state.o \
./src/ull_sub_state.o \
./src/vf2_mono_state.o \
./src/vf2_state.o \
./src/vf2_sub_state.o \
./src/vf_mono_state.o \
./src/vf_state.o \
./src/vf_sub_state.o \
./src/xsubgraph.o 

CPP_DEPS += \
./src/Matching.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


