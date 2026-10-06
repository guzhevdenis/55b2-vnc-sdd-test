############################################################################
## User provided PLC or fast controller driver pre configuration
############################################################################

# Enable parallel callback threads to improve 'I/O Intr' record scanning
#    see https://bugzilla.iter.org/codac/show_bug.cgi?id=10413
callbackParallelThreads

ndsLoadDriver $(PWD)/../c++/nds3-sender/target/lib/Nds3Sender.so
ndsCreateDevice(Nds3Sender, B-5)

#- End-of-file marker - do not delete or add lines below!
