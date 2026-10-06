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

DB+=nisyncDevice6683H-basic.substitutions
DB+=nisyncDevice6683H-terminal.substitutions
DB+=nisync_Device.template
DB+=nisync_Firmware.template
DB+=nisync_Terminal.template
DB+=nisync_Timing.template
DB+=nisync_Routing.template

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
