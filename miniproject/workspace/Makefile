################################################################################
# Automatically-generated file. Do not edit!
################################################################################
#program executable name
# program executable name
TARGET = /export/nfs/root/workspace/miniproject/main
# compiler flags
CFLAGS = -g -Wall
# linker flags
LDFLAGS = -g -lrt -pthread
# list of sources
SOURCES = $(shell find -name "*.c")
# default rule, to compile everything
all: $(TARGET)
# define object files
OBJECTS = $(SOURCES:.c=.o)
#link programs 
$(TARGET):$(OBJECTS)
	/home/student/Desktop/grupp1/TTK4147SANNTIDSYSTEMER/ex8/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc  -o $@ $^ $(LDFLAGS) 

#compile 
%.o : %.c
	/home/student/Desktop/grupp1/TTK4147SANNTIDSYSTEMER/ex8/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc $(CFLAGS) -c -o $@ $<

#cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)
	

# -include ../makefile.init

# RM := rm -rf

# # All of the sources participating in the build are defined here
# -include sources.mk
# -include subdir.mk
# -include objects.mk

# ifneq ($(MAKECMDGOALS),clean)
# ifneq ($(strip $(C_DEPS)),)
# -include $(C_DEPS)
# endif
# endif

# -include ../makefile.defs

# # Add inputs and outputs from these tool invocations to the build variables 

# # All Target
# all: main

# # Tool invocations
# main: $(OBJS) $(USER_OBJS)
# 	@echo 'Building target: $@'
# 	@echo 'Invoking: Cross GCC Linker'
# 	/home/student/Desktop/grupp1/TTK4147SANNTIDSYSTEMER/ex8/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc  -o "main" $(OBJS) $(USER_OBJS) $(LIBS)
# 	#avr32-linux-gcc  -o "main" $(OBJS) $(USER_OBJS) $(LIBS)
	
# 	@echo 'Finished building target: $@'
# 	@echo ' '

# # Other Targets
# clean:
# 	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) main
# 	-@echo ' '

# .PHONY: all clean dependents
# .SECONDARY:

# -include ../makefile.targets