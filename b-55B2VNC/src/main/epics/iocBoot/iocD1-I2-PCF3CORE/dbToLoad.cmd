
#======================================================================
# Loading DBs
#======================================================================
cd "$(TOP)/db"
dbLoadRecords("PCF3-D1-I2-B2A0-DAQ-ADC-iocD1-I2-PCF3CORE.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DAQ-PPC-iocD1-I2-PCF3CORE.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DME-APC.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DME-BCO.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DME-MPC.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DME-PHY.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DPC-ANA-iocD1-I2-PCF3CORE.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DPC-CAL-iocD1-I2-PCF3CORE.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DPC-CFG-iocD1-I2-PCF3CORE.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DPC-MES-iocD1-I2-PCF3CORE.db")
dbLoadRecords("PCF3-D1-I2-B2A0-DPC-MON-iocD1-I2-PCF3CORE.db")
dbLoadRecords("PCF3-D1-I2-B2A0-GLB-COD.db")
dbLoadRecords("PCF3-D1-I2-B2A0-GLB-DIG.db")
dbLoadRecords("PCF3-D1-I2-B2A0-GLB-PCS.db")
dbLoadRecords("PCF3-D1-I2-B2A0-OPR-AUT.db")
dbLoadRecords("PCF3-D1-I2-B2A0-OPR-CFG.db")
dbLoadRecords("PCF3-D1-I2-B2A0-OPR-INT-iocD1-I2-PCF3CORE.db")
dbLoadRecords("PCF3-D1-I2-B2A0-OPR-PST.db")
dbLoadRecords("PCF3-D1-I2-B2A0-SGC-DSP-iocD1-I2-PCF3CORE.db")
dbLoadRecords("PCF3-D1-I2-SYSM-iocD1-I2-PCF3CORE.db")

# Nds3Receiver driver - receives CountRate from PCF4CORE sender
dbLoadRecords("Nds3Receiver.db", "PREFIX=PCF3-D1-I2-B2A0,DEVICE=B-6,SOURCE=PCF4-D1-I2-B2A0-CountRate")

#- End-of-file marker - do not delete or add lines below!
