
#======================================================================
# Loading DBs
#======================================================================
cd "$(TOP)/db"
dbLoadRecords("PCF0-D1-I2-B2B0-DAQ-ADC-iocD1-I2-PCF0CORE.db")
dbLoadRecords("PCF0-D1-I2-B2B0-DAQ-PPC-iocD1-I2-PCF0CORE.db")
dbLoadRecords("PCF0-D1-I2-B2B0-DPC-ANA-iocD1-I2-PCF0CORE.db")
dbLoadRecords("PCF0-D1-I2-B2B0-DPC-CAL-iocD1-I2-PCF0CORE.db")
dbLoadRecords("PCF0-D1-I2-B2B0-DPC-CFG-iocD1-I2-PCF0CORE.db")
dbLoadRecords("PCF0-D1-I2-B2B0-DPC-MES-iocD1-I2-PCF0CORE.db")
dbLoadRecords("PCF0-D1-I2-B2B0-DPC-MON-iocD1-I2-PCF0CORE.db")
dbLoadRecords("PCF0-D1-I2-B2B0-SGC-DSP-iocD1-I2-PCF0CORE.db")
dbLoadRecords("PCF0-D1-I2-SYSM-iocD1-I2-PCF0CORE.db")

#dbLoadRecords("PCF0-D1-I2-B2B0-NDS.db")
dbLoadRecords("Ni5763_NDS_LabVIEW_Variables.db")
dbLoadRecords("Ni5763_NDS_DataAcquisition.db")
dbLoadTemplate("StateMachine.substitutions")
dbLoadTemplate("Ni5763_NDS_DANCP.substitutions")
# dbLoadTemplate("nisyncDevice6683H-basic.substitutions")
# dbLoadTemplate("nisyncDevice6683H-terminal.substitutions")


#- End-of-file marker - do not delete or add lines below!
