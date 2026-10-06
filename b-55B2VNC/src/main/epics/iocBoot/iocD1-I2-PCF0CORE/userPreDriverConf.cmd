############################################################################
## User provided PLC or fast controller driver pre configuration
############################################################################

# Enable parallel callback threads to improve 'I/O Intr' record scanning
#    see https://bugzilla.iter.org/codac/show_bug.cgi?id=10413
callbackParallelThreads

# ndsLoadDriver $(PWD)/../c++/nds3-ni5772/target/lib/libnds3-Ni5772Digitizer.so
# ndsCreateDevice Ni5772Digitizer B-0

# ndsLoadDriver $(PWD)/../c++/nds3-6683h/target/lib/libnds3-dev-nisync.so
# ndsCreateDevice(nisyncDevice, "B-1","TERMINALS=0-18","AUTOENABLE=running")

ndsLoadDriver $(PWD)/../c++/nds3-ni5763/target/lib/libnds3-Ni5763.so
ndsCreateDevice Ni5763 B-8

ndsLoadDriver $(PWD)/../c++/DANDrivers/Ni5763DANCP/lib/libnds-dan-ni5763.so
ndsCreateDevice(Ni5763DANCP, DAN-Ni5763, "INIT=YES")

nds subscribe DAN-Ni5763-buffer B-8-DataAcquisitionNode_FissCh-Data
## Load record instances
#- End-of-file marker - do not delete or add lines below!
