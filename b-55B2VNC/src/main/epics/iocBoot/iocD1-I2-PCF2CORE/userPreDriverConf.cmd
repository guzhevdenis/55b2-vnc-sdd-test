############################################################################
## User provided PLC or fast controller driver pre configuration
############################################################################

# Enable parallel callback threads to improve 'I/O Intr' record scanning
#    see https://bugzilla.iter.org/codac/show_bug.cgi?id=10413
callbackParallelThreads

ndsLoadDriver $(PWD)/../c++/DANDataGenerator/target/lib/DanDataGenerator.so
ndsCreateDevice(DanDataGenerator, G-0, "INIT=YES")

#ndsLoadDriver $(PWD)/../c++/DANDriver/DeviceDANCopy/lib/libnds-dan-device-Copy.so
#ndsCreateDevice(DeviceNDSDANCopy, DAN-0, "INIT=YES")

#nds subscribe DAN-0-buffer G-0-DataAcquisitionNode-Data

#- End-of-file marker - do not delete or add lines below!
