CXX = g++
CC = gcc

CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL -I/opt/codac/include -I./include/  -I./ -I/usr/lib/ -I/usr/include/ -g
CFLAGS = -Wall -Wextra -fPIC -pthread -DNDS3_DLL -I/opt/codac/include -I./include/  -I./ -I/usr/lib/ -I/usr/include/ -g
LDFLAGS =  -L/opt/codac/lib

TARGETDIR = ./target/lib
# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed  -Wl,-rpath=.

# Name of the nds library
TARGET = libnds3-OscilProcessing.so

# Additional linker libraries
LIBS = -lnds3 -lNiFpga -ldl -lniflexrio

# Source code files used in this project
CPP_SRCS = ./src/OscilProcessing.cpp ./src/DataAcquisitionNode_Ch0.cpp ./src/DataAcquisitionNode_Ch1.cpp ./src/OscilStateMachineNode.cpp 
C_SRCS = ./src/NiFpga_NI5772_Oscilloscope_FPGA_Top.c
OBJS = $(CPP_SRCS:.cpp=.o) $(C_SRCS:.c=.o)


.INTERMEDIATE: $(OBJS)

all: $(TARGETDIR)/$(TARGET) 

# Rules for building
$(TARGETDIR)/$(TARGET): $(OBJS)
	mkdir -p $(TARGETDIR)
	$(CXX) $(LINK) $(LDFLAGS) -o $@ $^ $(LIBS)


.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
	$(RM) $(TARGETDIR)/$(TARGET) 