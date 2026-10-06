
#======================================================================
# Loading DBs
#======================================================================
cd "$(TOP)/db"
dbLoadRecords("PCF4-D1-I2-B2A0-DAQ-ADC-iocD1-I2-PCF4CORE.db")
dbLoadRecords("PCF4-D1-I2-B2A0-DAQ-PPC-iocD1-I2-PCF4CORE.db")
dbLoadRecords("PCF4-D1-I2-B2A0-DPC-ANA-iocD1-I2-PCF4CORE.db")
dbLoadRecords("PCF4-D1-I2-B2A0-DPC-CAL-iocD1-I2-PCF4CORE.db")
dbLoadRecords("PCF4-D1-I2-B2A0-DPC-CFG-iocD1-I2-PCF4CORE.db")
dbLoadRecords("PCF4-D1-I2-B2A0-DPC-MES-iocD1-I2-PCF4CORE.db")
dbLoadRecords("PCF4-D1-I2-B2A0-DPC-MON-iocD1-I2-PCF4CORE.db")
dbLoadRecords("PCF4-D1-I2-B2A0-OPR-INT-iocD1-I2-PCF4CORE.db")
dbLoadRecords("PCF4-D1-I2-B2A0-SGC-DSP-iocD1-I2-PCF4CORE.db")
dbLoadRecords("PCF4-D1-I2-SYSM-iocD1-I2-PCF4CORE.db")

# Nds3Sender driver
dbLoadRecords("Nds3Sender.db", "PREFIX=PCF4-D1-I2-B2A0,DEVICE=B-5")

#- End-of-file marker - do not delete or add lines below!
