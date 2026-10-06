
#======================================================================
# Loading DBs
#======================================================================
cd "$(TOP)/db"
dbLoadRecords("PCF5-D1-I2-B2A0-DAQ-ADC-iocD1-I2-PCF5CORE.db")
dbLoadRecords("PCF5-D1-I2-B2A0-DAQ-PPC-iocD1-I2-PCF5CORE.db")
dbLoadRecords("PCF5-D1-I2-B2A0-DPC-ANA-iocD1-I2-PCF5CORE.db")
dbLoadRecords("PCF5-D1-I2-B2A0-DPC-CAL-iocD1-I2-PCF5CORE.db")
dbLoadRecords("PCF5-D1-I2-B2A0-DPC-CFG-iocD1-I2-PCF5CORE.db")
dbLoadRecords("PCF5-D1-I2-B2A0-DPC-MES-iocD1-I2-PCF5CORE.db")
dbLoadRecords("PCF5-D1-I2-B2A0-DPC-MON-iocD1-I2-PCF5CORE.db")
dbLoadRecords("PCF5-D1-I2-B2A0-OPR-INT-iocD1-I2-PCF5CORE.db")
dbLoadRecords("PCF5-D1-I2-B2A0-SGC-DSP-iocD1-I2-PCF5CORE.db")
dbLoadRecords("PCF5-D1-I2-SYSM-iocD1-I2-PCF5CORE.db")

# Nds-Oscliloscope driver
dbLoadRecords("DataAcquisition_Ch0.db")
dbLoadRecords("DataAcquisition_Ch1.db")
dbLoadRecords("OscilloscopeData.db")
dbLoadRecords("OscilNDS-SDN.db")
dbLoadTemplate("OscilStateMachine.substitutions")
dbLoadTemplate("OscilDANCP.substitutions")

#- End-of-file marker - do not delete or add lines below!
