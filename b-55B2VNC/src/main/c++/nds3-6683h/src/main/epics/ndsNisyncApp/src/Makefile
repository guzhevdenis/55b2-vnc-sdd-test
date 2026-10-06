TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
# Build the IOC application

PROD_IOC += ndsNisync
USR_CXXFLAGS = -std=c++11 -Wextra -pedantic

# ndsex1.dbd will be created and installed
DBD += ndsNisync.dbd

# ndsIoc.dbd will be made up from these files:
#USR_DBDFLAGS=-I../../../../dbd
USR_DBDFLAGS= -I$(TOP)/../dbd
ndsNisync_DBD += base.dbd
ndsNisync_DBD += asyn.dbd
ndsNisync_DBD += nds3-epics.dbd
#adding the support for asynDriver
-include $(EPICS_BASE)/mk/asyn.mk
#adding the support for nds3-epics
-include $(EPICS_BASE)/mk/nds3-epics.mk

# Add all the support libraries needed by this IOC
#nds3-epics_DIR += ../../../../lib/linux-x86_64
nds3-epics_DIR += $(TOP)/../lib/$(T_A)
ndsNisync_LIBS += $(CODAC_LIBS)
#ndsNisync_LIBS +=  nisync
#ndsNisync_SYS_LIBS += nds3
#ndsNisync_SYS_LIBS += asyn
#ndsNisync_SYS_LIBS += nds3-epics

# Enforce linking. This ii using rpath and this is not correct. If removed execution error because the library is not found
ndsNisync_LDFLAGS += -Wl,-rpath=$(CURDIR)/../../../../lib/linux-x86_64


# ndsex1_registerRecordDeviceDriver.cpp derives from ndsex1.dbd
ndsNisync_SRCS += ndsNisync_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ndsNisync_SRCS_DEFAULT += ndsNisyncMain.cpp
ndsNisync_SRCS_vxWorks += -nil-


# Finally link to the EPICS Base libraries
ndsNisync_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

