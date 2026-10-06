
#======================================================================
# Loading DBs
#======================================================================
cd "$(TOP)/db"
dbLoadRecords("PCF2-D1-I2-B2B0-DAQ-ADC-iocD1-I2-PCF2CORE.db")
dbLoadRecords("PCF2-D1-I2-B2B0-DAQ-PPC-iocD1-I2-PCF2CORE.db")
dbLoadRecords("PCF2-D1-I2-B2B0-DPC-ANA-iocD1-I2-PCF2CORE.db")
dbLoadRecords("PCF2-D1-I2-B2B0-DPC-CAL-iocD1-I2-PCF2CORE.db")
dbLoadRecords("PCF2-D1-I2-B2B0-DPC-CFG-iocD1-I2-PCF2CORE.db")
dbLoadRecords("PCF2-D1-I2-B2B0-DPC-MES-iocD1-I2-PCF2CORE.db")
dbLoadRecords("PCF2-D1-I2-B2B0-DPC-MON-iocD1-I2-PCF2CORE.db")
dbLoadRecords("PCF2-D1-I2-B2B0-SGC-DSP-iocD1-I2-PCF2CORE.db")
dbLoadRecords("PCF2-D1-I2-SYSM-iocD1-I2-PCF2CORE.db")

dbLoadTemplate("/home/codac-dev/VNC_development_Denis/55B2VNC/b-55B2VNC/src/main/epics/iocBoot/iocD1-I2-PCF2CORE/DeviceNDSDANCP.substitutions")


#- End-of-file marker - do not delete or add lines below!
