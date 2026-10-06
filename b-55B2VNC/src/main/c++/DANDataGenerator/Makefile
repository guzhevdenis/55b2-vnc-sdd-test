CXX = g++
 
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -fPIC -pthread -DNDS3_DLL -I/opt/codac/include -I./include/  -I./ -I/usr/lib/ -I/usr/include/ -g
LDFLAGS =  -L/opt/codac/lib 

TARGETDIR = ./target/lib
# Flags passed to gcc during linking
LINK = -shared -fPIC -Wl,-as-needed  -Wl,-rpath=.
 
# Name of the nds library
TARGET = DanDataGenerator.so
 
# Additional linker libraries
LIBS = -lnds3 -ldl 
 
# Source code files used in this project
SRCS = ./src/DanDataGenerator.cpp ./src/DataAcquisitionNode.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGETDIR)/$(TARGET) 

# Rules for building
$(TARGETDIR)/$(TARGET): $(OBJS)
	mkdir -p $(TARGETDIR)
	$(CXX) $(LINK) $(LDFLAGS) -o $@ $^ $(LIBS)


.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
	$(RM) $(TARGETDIR)/$(TARGET) 
