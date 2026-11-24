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

#DBs for nds-oscilloscope driver	
DB += Oscil_NDS_StateMachine.substitutions
DB += Oscil_NDS_LabVIEW_Variables.db
DB += Oscil_NDS_DataAcquisition_Ch0.db
DB += Oscil_NDS_DataAcquisition_Ch1.db

#DBs for nds-dan-cp driver
DB += Oscil_NDS_DANCP.substitutions

#DBs for nds-sdn driver
DB += Oscil_NDS_SDN.db

# Automagically install .proto files
DB += $(notdir $(wildcard ../*.proto))

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE