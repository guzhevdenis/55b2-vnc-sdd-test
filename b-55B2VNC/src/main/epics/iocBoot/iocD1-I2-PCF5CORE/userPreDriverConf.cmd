############################################################################
## User provided PLC or fast controller driver pre configuration
############################################################################

# Enable parallel callback threads to improve 'I/O Intr' record scanning
#    see https://bugzilla.iter.org/codac/show_bug.cgi?id=10413
callbackParallelThreads

ndsLoadDriver $(PWD)/../c++/nds-oscilloscope/target/lib/libnds3-OscilProcessing.so
ndsCreateDevice OscilProcessing B-7

#ndsLoadDriver $(PWD)/../c++/nds3-6683h/target/lib/libnds3-dev-nisync.so
#ndsCreateDevice(nisyncDevice, "B-1","TERMINALS=0-18","AUTOENABLE=running")

ndsLoadDriver $(PWD)/../c++/DANDrivers/OscilDANCP/lib/libnds-dan-oscil-device-Copy.so
ndsCreateDevice(OscilDANCP, DAN-1, "INIT=YES")

nds subscribe DAN-1-buffer_Ch0 B-7-DataAcquisitionNode_Ch0-Data
nds subscribe DAN-1-buffer_Ch1 B-7-DataAcquisitionNode_Ch1-Data

ndsLoadDriver $(PWD)/../c++/nds-sdn-oscil/lib/libnds3-sdn-oscil.so
ndsCreateDevice(DeviceNDSSDNPUB, "SDN-0","INTERFACE=lo","TOPICNAME=topic-nds-oscil","PVS_CONFIG_PATH=../resources/input.xml")

#- End-of-file marker - do not delete or add lines below!