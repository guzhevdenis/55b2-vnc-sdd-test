CXX =g++

STD =-std=c++11
INCLUDES =-I./include/ -I/opt/codac/include
DEFS =-DEPICS -DNDS3_DLL
CXXFLAGS =$(STD) -Wall -Wextra -pedantic -fPIC -pthread $(DEFS)
LDFLAGS =-L../../../lib -L/opt/codac/lib -g

#CXXFLAGS +=$(INCLUDES)
TARGETDIR =../../../lib
# Flags passed to gcc during linking
LINK = -shared -Wl,-as-needed
 
# List of targets 
TARGET = $(patsubst %.cpp,$(TARGETDIR)/libnds3-%.so,$(wildcard *.cpp))
#TARGET := $(patsubst %.cpp, %.so,$(wildcard *.cpp))
SOURCES:=$(wildcard *.cpp) 

# Additional linker libraries
LIBS =-lnds3
 
all: $(TARGET)

$(TARGETDIR)/libnds3-%.so: %.cpp
	$(CXX)  $(CXXFLAGS) $(INCLUDES) $(LINK) $(LDFLAGS) $< -o $@ $(LIBS)

clean:
	$(RM) $(TARGET) 