C=gcc
CXX=g++
INC_DIR1=.
INC_DIR2=/opt/codac/include
LIB_DIR=/opt/codac/lib
LIB=NiFpga

all:
	$(CXX) -I$(INC_DIR1) -I$(INC_DIR2) -L$(LIB_DIR) -l$(LIB) main.cpp -o Mean

run:
	./Mean
