TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install)
# databases, templates, substitutions like this
DB += OscilStateMachine.substitutions
DB += OscilDANCP.substitutions
DB += LocalPVs.template

# Automagically install .proto files
DB += $(notdir $(wildcard ../*.proto))

#DataAcquisition db file
DB += DataAcquisition_Ch0.db
DB += DataAcquisition_Ch1.db
DB += OscilNDS-SDN.db
#OscilloscopeData db file
DB += OscilloscopeData.db

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE