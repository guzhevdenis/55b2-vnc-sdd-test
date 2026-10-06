#+======================================================================
# $HeadURL$
# $Id$
#
# Project       : CODAC Core System
#
# Description   : Makefile.
#
# Authors       : James Blair (National Instruments)
#                 Mike Hoogendyk (National Instruments)
#
# Copyright (c) : 2013-2017 ITER Organization,
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

TEST = nds3driver-nisync

NDSLIBDIR:= $(realpath ../../../lib)
# Path from which the device source file will be obtained.
EXAMPLESDIR:=$(realpath ../../../main/c++/nisyncDriver)
EXAMPLESHEADERSDIR:=$(wildcard $(EXAMPLESDIR)/include)
# Necessary for make to find the source files in a different directory.
VPATH =$(EXAMPLESDIR)

INCLUDE_DIRS := $(realpath ./include ../../../main/c++/include $(CODAC_ROOT)/include $(CODAC_ROOT)/include/common $(EXAMPLESHEADERSDIR))
#LIB_DIRS := $(realpath ../../../../target/main/c++/$(PVXS_VERSION)/lib/linux-x86_64)
INCLUDES :=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))
LIBPATH :=-L$(NDSLIBDIR) -Wl,-rpath=$(NDSLIBDIR) -L$(CODAC_ROOT)/lib

#############################################################################


CXXFLAGS +=$(INCLUDES) -MMD -MP -Wall -std=c++11
LDFLAGS +=$(LIBPATH) -lpthread -lnds3 -lnds3-testcontrolsystem  -ldl -lrt -lgtest -lnds3-dev-nisync

CXXFLAGS+= -DDEBUG -O0 -g --coverage
LDFLAGS+= --coverage

#TESTS_OBJS := $(patsubst $(EXAMPLESDIR)/%.cpp, %.o, $(wildcard $(EXAMPLESDIR)/*.cpp))
TESTS_OBJS += $(patsubst %.cpp, %.o, $(wildcard *.cpp))

all: $(TEST)

$(TEST): $(TESTS_OBJS) 
	$(CXX) $^ $(CXXFLAGS) -o $@ $(LDFLAGS)

clean:
	@$(RM) $(TESTS) *.o *.d
