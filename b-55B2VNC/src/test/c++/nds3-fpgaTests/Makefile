# Makefile for NI5772Digitizer unit tests

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g -fPIC
LDFLAGS = 

# Include directories
NDS3_INCLUDE = -I../../../../main/c++/nds3-ni5772/include \
               -I../../../../main/c++/nds3-ni5772/include/nds-dan \
               -I$(CODAC_ROOT)/include/nds3

# Library directories
NDS3_LIBDIR = -L$(CODAC_ROOT)/lib \
              -L../../../../main/c++/nds3-ni5772/target/lib

# Libraries
LIBS = -lnds3 -lgtest -lgtest_main -lpthread -lNiFpga

# Source files
TEST_SOURCES = TP_NI5772_0000.cpp \
               TP_NI5772_0001.cpp \
               TP_NI5772_0002.cpp \
               TP_NI5772_0003.cpp \
               TP_NI5772_0004.cpp \
               main.cpp

# Object files
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# Executable
TEST_EXE = ni5772Tests

.PHONY: all clean test run

all: $(TEST_EXE)

$(TEST_EXE): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(NDS3_LIBDIR) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(NDS3_INCLUDE) -c $< -o $@

test: $(TEST_EXE)
	./$(TEST_EXE)

run: test

clean:
	rm -f $(TEST_OBJECTS) $(TEST_EXE)

help:
	@echo "Available targets:"
	@echo "  all     - Build test executable"
	@echo "  test    - Build and run tests"
	@echo "  run     - Alias for test"
	@echo "  clean   - Remove build artifacts"
	@echo ""
	@echo "Environment variables:"
	@echo "  CODAC_ROOT - Path to CODAC Core System installation"


