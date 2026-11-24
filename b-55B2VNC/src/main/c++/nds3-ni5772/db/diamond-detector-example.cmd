# Load before iocInit in an IOC with nds3-epics/asyn support.
# Set NI5772_DRIVER to the absolute nds3-ni5772 source/install directory first:
# epicsEnvSet("NI5772_DRIVER", "/absolute/path/to/nds3-ni5772")
# Adjust PORT, PREFIX and RESOURCE to the installation.
# Also set EPICS_CA_MAX_ARRAY_BYTES=4194304 on waveform CA clients.
epicsEnvSet("NI5772_PORT", "B-0")
epicsEnvSet("NI5772_PREFIX", "D1-I2-HWCF:B-0-")
epicsEnvSet("EPICS_CA_MAX_ARRAY_BYTES", "4194304")

ndsLoadDriver "$(NI5772_DRIVER)/target/lib/libnds3-Ni5772Digitizer.so"
ndsCreateDevice(Ni5772Digitizer, "$(NI5772_PORT)", "BITFILE=$(NI5772_DRIVER)/bitfile/NiFpga_DiamondDetectorDAQ_FPGA_Top.lvbitx", "RESOURCE=RIO0", "INIT=OFF")

dbLoadRecords("$(NI5772_DRIVER)/db/DiamondDetectorDAQ.template", "P=$(NI5772_PREFIX),PORT=$(NI5772_PORT)")
dbLoadRecords("$(NI5772_DRIVER)/db/DiamondDetectorDAQ-channel.template", "P=$(NI5772_PREFIX),PORT=$(NI5772_PORT),CH=0")
dbLoadRecords("$(NI5772_DRIVER)/db/DiamondDetectorDAQ-channel.template", "P=$(NI5772_PREFIX),PORT=$(NI5772_PORT),CH=1,MIN_LO=-32768,MIN_HI=32767")
