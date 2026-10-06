############################################################################
## User provided PLC or fast controller driver pre configuration
############################################################################

# Enable parallel callback threads to improve 'I/O Intr' record scanning
#    see https://bugzilla.iter.org/codac/show_bug.cgi?id=10413
callbackParallelThreads

ndsLoadDriver $(PWD)/../c++/DataAgregator/target/lib/libnds3-DataAgregator.so
ndsCreateDevice DataAgregator B-3

ndsLoadDriver $(PWD)/../c++/DataGenerator/target/lib/libnds3-DataGenerator.so
ndsCreateDevice DataGenerator B-4

nds subscribe B-3-Temperature00-received B-4-Temperature00
nds subscribe B-3-Temperature01-received B-4-Temperature01
nds subscribe B-3-Temperature02-received B-4-Temperature02
nds subscribe B-3-Temperature03-received B-4-Temperature03
nds subscribe B-3-Temperature04-received B-4-Temperature04
nds subscribe B-3-Temperature05-received B-4-Temperature05
nds subscribe B-3-Temperature06-received B-4-Temperature06
nds subscribe B-3-Temperature07-received B-4-Temperature07
nds subscribe B-3-Temperature08-received B-4-Temperature08
nds subscribe B-3-Temperature09-received B-4-Temperature09
nds subscribe B-3-Temperature10-received B-4-Temperature10
nds subscribe B-3-Temperature11-received B-4-Temperature11
nds subscribe B-3-Temperature12-received B-4-Temperature12
nds subscribe B-3-Temperature13-received B-4-Temperature13
nds subscribe B-3-Temperature14-received B-4-Temperature14
nds subscribe B-3-Temperature15-received B-4-Temperature15
nds subscribe B-3-Temperature16-received B-4-Temperature16
nds subscribe B-3-Temperature17-received B-4-Temperature17
nds subscribe B-3-Temperature18-received B-4-Temperature18
nds subscribe B-3-Temperature19-received B-4-Temperature19
nds subscribe B-3-Temperature20-received B-4-Temperature20
nds subscribe B-3-Temperature21-received B-4-Temperature21
nds subscribe B-3-Temperature22-received B-4-Temperature22
nds subscribe B-3-Temperature23-received B-4-Temperature23
#- End-of-file marker - do not delete or add lines below!
