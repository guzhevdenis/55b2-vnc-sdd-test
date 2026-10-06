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

TEST = nds3driver-nds-sdn

NDSLIBDIR:=$(realpath ../../../../target/lib)

# Path from which the device source file will be obtained.
EXAMPLESDIR:=$(realpath ../../../main/c++/DeviceTesting)
EXAMPLESHEADERSDIR:=$(realpath $(EXAMPLESDIR)/include)

# Necessary for make to find the source files in a different directory.
VPATH =$(EXAMPLESDIR)


INCLUDES :=-I ../include  -I ../../../main/c++/nds-sdn/include -I/opt/codac/include -I /usr/include/libxml2 -I $(EXAMPLESHEADERSDIR)
LIBPATH :=-L../../../lib -Wl,-rpath=$(NDSLIBDIR) -L/opt/codac/lib 

#############################################################################

CXXFLAGS +=$(INCLUDES) -MMD -MP -Wall -std=c++11

ifeq ($(COVERAGE),true)
  CFLAGS+= -O0 -g --coverage
  CXXFLAGS+= -O0 -g --coverage
  LDFLAGS+= --coverage
endif
LDFLAGS +=$(LIBPATH) -lpthread -lnds3-testcontrolsystem -lnds3 -ldl -lrt -lgtest -lnds3-sdn-publisher -lnds3-DeviceTesting -lccs-core
TESTS_OBJS := $(patsubst %.cpp, %.o, $(wildcard *.cpp))

all: $(TEST)

$(TEST): $(TESTS_OBJS) 
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	@$(RM) $(TESTS) *.o *.d
