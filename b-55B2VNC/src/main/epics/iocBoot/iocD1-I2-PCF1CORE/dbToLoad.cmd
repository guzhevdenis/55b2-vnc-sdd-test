
#======================================================================
# Loading DBs
#======================================================================
cd "$(TOP)/db"
dbLoadRecords("PCF1-D1-I2-B2B0-DAQ-ADC-iocD1-I2-PCF1CORE.db")
dbLoadRecords("PCF1-D1-I2-B2B0-DAQ-PPC-iocD1-I2-PCF1CORE.db")
dbLoadRecords("PCF1-D1-I2-B2B0-DPC-ANA-iocD1-I2-PCF1CORE.db")
dbLoadRecords("PCF1-D1-I2-B2B0-DPC-CAL-iocD1-I2-PCF1CORE.db")
dbLoadRecords("PCF1-D1-I2-B2B0-DPC-CFG-iocD1-I2-PCF1CORE.db")
dbLoadRecords("PCF1-D1-I2-B2B0-DPC-MES-iocD1-I2-PCF1CORE.db")
dbLoadRecords("PCF1-D1-I2-B2B0-DPC-MON-iocD1-I2-PCF1CORE.db")
dbLoadRecords("PCF1-D1-I2-B2B0-SGC-DSP-iocD1-I2-PCF1CORE.db")
dbLoadRecords("PCF1-D1-I2-SYSM-iocD1-I2-PCF1CORE.db")
dbLoadRecords("dataProcessing.db")

#- End-of-file marker - do not delete or add lines below!
