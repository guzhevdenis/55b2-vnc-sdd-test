############################################################################
## User provided PLC or fast controller driver pre configuration
############################################################################

# Enable parallel callback threads to improve 'I/O Intr' record scanning
#    see https://bugzilla.iter.org/codac/show_bug.cgi?id=10413
callbackParallelThreads

ndsLoadDriver $(PWD)/../c++/nds3-receiver/target/lib/Nds3Receiver.so
ndsCreateDevice(Nds3Receiver, B-6)

#- End-of-file marker - do not delete or add lines below!
