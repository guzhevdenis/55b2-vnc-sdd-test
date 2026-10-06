# 55B2VNC

## Description

The software project for VNC Data Acquisition system.
The project configuration was initially created using the Self Description Data (SDD) Editor tool, which defines the overall system architecture — including the number and type of Plant Controllers Fast (PCFs), PLCs, Data Acquisition Boards, and other components.

The complete I&C project structure is contained in the b-55B2VNC directory.

In SDD-xmls the configuration files (variables, component definition, additional device description files) are located. It is used during SDD project generation.

In SDN_VNC_Publisher_Subscriber test project for SDN interface. 

The main software elements include:

- EPICS IOCs and their configuration files — located in
b-55B2VNC/src/main/epics

- NDS-Drivers — implemented in C++ and located in
b-55B2VNC/src/main/c++. 

- The SNL program that manages and controls the state transitions defined in PSOS is located in
b-55B2VNC/src/main/epics/D1-I2App/src/.

- HMI screens developed with HMI Guideline for Diagnostic I&C systems (9X7BNF) in b-55B2VNC/src/main/boy/mimics and navigation files to these screens in b-55B2VNC/src/main/boy/navigation. (The HMIs has been tested with PCF0CORE IOC).


Now only 5 EPICS IOCs are used.

D1-I2-PCF0CORE - is for data processing from LabVIEW Projects. It uses NDS-drivers nds3-ni5772 (for fpga boards) and nds3-dev-nisync (for 6683h).
D1-I2-PCF1CORE - is for test purposes of plasma parameter calculation with 2 NDS-drivers and integration of machine learning model.
D1-I2-PCF2CORE - is for DAN API test purposes.
D1-I2-PCF3CORE - is for Data Receiver NDS-Driver from another PCF. / test purposes 
D1-I2-PCF4CORE - is for Data Sender NDS-Driver to another PCF . / test purposes
D1-I2-PCF5CORE - is for displaying oscilloscope data from LabVIEW Project. It uses NDS-driver nds-oscilloscope and nds-dan driver. 


## Visuals
The scheme of High-Level Architecture is presented in VNC_IOCs_Location.PNG. The scheme of low level architecture is presented in FastController_softwareArchitecture.png.

## Installation
The project is designed for CODAC Core System version 7.2.1.
This version already includes support for NDSv3 and National Instruments Linux Drivers, which are required for integration with the NI hardware used in this system.

No additional setup is needed beyond the standard CODAC Core System environment.

## Usage

To run the basic IOC, navigate to the project directory:
```bash
mvn run -Dioc=PCF0CORE 
```
To stop the IOC, either use the IOC shell:
exit or from another terminal session:

```bash
 mvn stop -Dioc=PCF0CORe
 ```
To run the full project:

```bash
mvn run
```


To create or load this project in the SDD Editor, use the XML configuration files located in the SDD-xmls directory.
Before doing so, make sure that the National Instruments PXIe-7976R FPGA board with the NI-5772 ADC module, 
as well as the NI-5763 board, are properly registered in the SDD hardware database.

The following hardware description files provide the required definitions:
HardwarePXIe7976RNI5772.xml
HardwareNIPXI5763.xml

To include them in the SDD database, execute the following command from a terminal:
```bash
/opt/codac/bin/sdd-sync-classic load HardwareNIPXI5763.xml
/opt/codac/bin/sdd-sync-classic load HardwarePXIe7976RNI5772.xml
```

Once both hardware files are loaded, the project XML files in SDD-xmls can be opened and edited in the SDD Editor.

If you edit the EPICS DB files and you need to update the records in SDD database, use:
mvn retrofit.

To add the developed NDS-Device Support, use the manual NDS v3 EPICS Device Support User Manual (S977XP):

- Configure the application to use the NDS-Core and NDS-EPICS Software 

in b-55B2VNC/src/main/epics/PCF0CoreApp/src add these lines:
```bash
#adding the support for asynDriver
-include $(EPICS_ROOT)/mk/asyn.mk
#adding the support for nds3-epics
-include $(EPICS_ROOT)/mk/nds3-epics.mk
```

- Configure IOC and userPredriverconfig.cmd file
```bash
#Load the shared library driver
ndsLoadDriver $(PWD)/../c++/nds3-ni5772/target/lib/libNdsVNC.so
ndsCreateDevice Device B-0

ndsLoadDriver $(PWD)/../c++/nds3-dan/target/lib/libnds-dan-device-Copy.so
ndsCreateDevice(DeviceNDSDANCopy, DAN-0, "INIT=YES")
nds subscribe DAN-0-buffer B-0-DataAcquisitionNode-Data

ndsLoadDriver $(PWD)/../c++/nds3-6683h/target/lib/libnds3-dev-nisync.so
ndsCreateDevice(nisyncDevice, "B-1","TERMINALS=0-18","AUTOENABLE=running")

#in /../iocBoot/iocD1-I2-PCF5CORE 
ndsLoadDriver $(PWD)/../c++/nds-oscilloscope/target/lib/libnds3-OscilProcessing.so
ndsCreateDevice OscilProcessing B-7
```
- Edit the envUser file 
```bash
epicsEnvSet("EPICS_DB_INCLUDE_PATH",".:$(TOP)/../db:$(EPICS_ROOT)/
db")
```

The process variables representing the PSOS states are defined in b-55B2VNC/src/main/epics/D1-I2App/Db/PSH0-D1-I2.db.
To include the SNL program in iocPSH0CORE, the seqToLoad.cmd file has been configured to load the sequence VNCPSOS.
Now it is developed as a simple framework for state transition. It is not conected to process variables that controls 
the hardware or implements real logic.  

To switch the state, use CMD variable is PSH0-D1-I2.db,
for example to switch into PowerOff State use:

```bash
caput D1-I2:POWEROFF-CMD 1
```

## Roadmap

- Integration of SDN developed support is needed. 

- Testing of Synchronous Databus Network (SDN) and Data Archiving Network (DAN) APIs.
Validate the system’s communication and data flow through SDN and DAN interfaces to ensure correct 
synchronization, data integrity, and compatibility with CODAC Core System services.

- Integration of developed software for Cubicle Monitoring and PLC application 
for power control modules. 

## Authors and acknowledgment
Denis Guzhev
Danila Avdushkin 

## License
This project is developed within the framework 
of the ITER Organization and follows the internal software distribution policies of the ITER CODAC System.

