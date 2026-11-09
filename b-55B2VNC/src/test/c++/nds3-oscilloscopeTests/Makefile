#+======================================================================
# $HeadURL: https://svnpub.codac.iter.org/codac/iter/codac/dev/units/m-nds-core/tags/CODAC-CORE-7.4.0/src/test/c++/nds-example/Makefile $
# $Id: Makefile 179115 2026-01-09 15:15:20Z kimh19 $
#
# Project       : CODAC Core System
#
# Description   : Makefile.
#
# Authors       : James Blair (National Instruments)
#                 Mike Hoogendyk (National Instruments)
#
# Copyright (c) : 2013-2026 ITER Organization,
#                 CS 90 046
#                 13067 St. Paul-lez-Durance Cedex
#                 France
#
# This file is part of ITER CODAC software.
# For the terms and conditions of redistribution or use of this software
# refer to the file ITER-LICENSE.TXT located in the top level directory
# of the distribution package.
#
#-======================================================================

NDSLIBDIR:=$(realpath ../../../main/c++/nds-oscilloscope/target/lib)
# Path from which the device source file will be obtained.
EXAMPLESDIR:=$(realpath ../../../main/c++/nds-oscilloscope/src)
# Necessary for make to find the source files in a different directory.
VPATH =$(EXAMPLESDIR)

# Path from which the device source file will be obtained once the example is installed in CODAC.
#EXAMPLESDIR+=$(realpath ../../../main/c++/nds3-ni5772/src)

EXAMPLESHEADERS:=$(wildcard $(EXAMPLESDIR)/../include/*.h)

INCLUDES :=-I../../../main/c++/include -I $(EXAMPLESDIR) -I../include -I/opt/codac/include
LIBPATH :=-L../../../lib -Wl,-rpath=$(NDSLIBDIR) -L/opt/codac/lib


#############################################################################


CXXFLAGS +=$(INCLUDES) -MMD -MP -g -Wall  -std=c++11 -pedantic -fPIC -pthread -DNDS3_DLL -I/opt/codac/include -I./include/  -I./ -I/usr/lib/ -I/usr/include/ -g
LDFLAGS +=$(LIBPATH) -lnds3-testcontrolsystem -lnds3 -lpthread -ldl -lrt -lgtest

#TESTS_OBJS := $(patsubst $(EXAMPLESDIR)/%.cpp, %.o, $(wildcard $(EXAMPLESDIR)/*.cpp))
#TEST_SRC = ./TP_OscilProcessing_0004.cpp ./main.cpp
#TESTS_OBJS += $(TEST_SRC:.cpp=.o)
TESTS_OBJS += $(patsubst %.cpp, %.o, $(wildcard *.cpp))

TEST =nds3-oscilloscopeTest

.PHONY: all clean

all: $(TEST)
	@$(RM) *.o *.d

$(TEST): $(TESTS_OBJS) 
	$(CXX) $^ --coverage -o $@ $(LDFLAGS)

clean:
	@$(RM) $(TEST) $(TESTS) *.o *.d