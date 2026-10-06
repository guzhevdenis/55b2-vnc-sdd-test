CXX = g++
 
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL -I/opt/codac/include -I./include/  -I./ -I/usr/lib/ -I/usr/include/ -g
LDFLAGS =  -L/opt/codac/lib 

TARGETDIR = ./target/lib
# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed  -Wl,-rpath=.
 
# Name of the nds library
TARGET = libnds3-Ni5772Digitizer.so
 
# Additional linker libraries
LIBS = -lnds3 -lNiFpga -ldl -lniflexrio
 
# Source code files used in this project
SRCS = ./src/Ni5772Digitizer.cpp ./src/DataAcquisitionNode.cpp ./src/DeviceStateMachineNode.cpp ./src/SignalProcessing.cpp
OBJS = $(SRCS:.cpp=.o)

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
