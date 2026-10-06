TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
# Build the IOC support library
#TODO install this library somewhere (changes in POM?)

#TODO Change this!! Harcoded path
#LIB_LDFLAGS += -L$(realpath $(TOP)/../../lib) -Wl,-rpath=$(realpath $(TOP)/../../lib) -lnds3-dev-nidaqx
#TODO Change this!! Harcoded path
#USR_INCLUDES += -I../../../../c++/nidaqxDriver/include

#=============================
# Build the IOC application

PROD_IOC += ndssdn
USR_CXXFLAGS = -std=c++11 -Wextra -pedantic -g

# ndsex1.dbd will be created and installed
DBD += ndssdn.dbd

# ndsIoc.dbd will be made up from these files:
#USR_DBDFLAGS=-I../../../../dbd
USR_DBDFLAGS= -I$(TOP)/../dbd
ndssdn_DBD += base.dbd
ndssdn_DBD += asyn.dbd
ndssdn_DBD += nds3-epics.dbd
#ndsNidaqx_DBD += ndsNidaqx.dbd
#adding the support for asynDriver
-include $(EPICS_BASE)/mk/asyn.mk
#adding the support for nds3-epics
-include $(EPICS_BASE)/mk/nds3-epics.mk

# Add all the support libraries needed by this IOC

ndssdn_DIR += $(TOP)/lib/$(T_A)
ndssdn_LIBS += $(CODAC_LIBS)



# Enforce linking. This ii using rpath and this is not correct. If removed execution error because the library is not found
ndssdn_LDFLAGS += -Wl,-rpath=$(realpath $(ndssdn_DIR))

# ndsNidaqx_registerRecordDeviceDriver.cpp derives from ndsNidaqx.dbd
ndssdn_SRCS += ndssdn_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
ndssdn_SRCS_DEFAULT += ndssdnMain.cpp
ndssdn_SRCS_vxWorks += -nil-


# Finally link to the EPICS Base libraries
ndssdn_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

