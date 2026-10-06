TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#========================================

# Use typed rset structure (see 3.16.1 release notes)
USR_CPPFLAGS += -DUSE_TYPED_RSET

#========================================
# Build the IOC application

PROD_IOC = PCF5Core
# PCF5Core.dbd will be created and installed
DBD += PCF5Core.dbd

# PCF5Core.dbd will be made up from these files:
-include $(EPICS_BASE)/mk/codac-common.mk


# Add all the support libraries needed by this IOC
-include $(EPICS_BASE)/mk/asyn.mk
-include $(EPICS_BASE)/mk/nisync-epics.mk
-include $(EPICS_BASE)/mk/nisync-generalTime.mk

PCF5Core_DBD += $(CODAC_DBD)
PCF5Core_LIBS += $(CODAC_LIBS)

# # SNCSEQ
# <sncProgram>_SNCFLAGS += +r -c +d
# PCF5Core_DBD += <sncProgram>.dbd
# PCF5Core_SRCS += <sncProgram>.stt
# PCF5Core_LIBS += seq pv


# PCF5Core_registerRecordDeviceDriver.cpp derives from PCF5Core.dbd
PCF5Core_SRCS += PCF5Core_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
PCF5Core_SRCS_DEFAULT += PCF5CoreMain.cpp
PCF5Core_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#PCF5Core_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

#=============================
# Include SNL program makefile snippets

-include ../*.snlprog

#=============================
# Include user.mk makefile snippets

-include ../user.mk

# Link QSRV (pvAccess Server) if available
ifdef EPICS_QSRV_MAJOR_VERSION
    PCF5Core_LIBS += qsrv
    PCF5Core_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
    PCF5Core_DBD += PVAServerRegister.dbd
    PCF5Core_DBD += qsrv.dbd
endif

#=============================
# Finally link to the EPICS Base libraries
PCF5Core_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
#----------------------------------------
#adding the support for asynDriver
-include $(EPICS_BASE)/mk/asyn.mk
#adding the support for nds3-epics
-include $(EPICS_BASE)/mk/nds3-epics.mk