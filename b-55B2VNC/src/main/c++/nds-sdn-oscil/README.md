## nds-sdn-oscil

NDS3-SDN driver for any other nds-driver 

## Description

This module provides an NDS-SDN driver that publishes data to SDN topics. Each instance receives data from another NDS driver and publishes it through the configured network interface according to an XML mapping.

The driver and its topic are configured with two XML files in `src/main/resources`:

- A driver configuration file that defines the NDS-SDN settings and PV-to-topic mapping.
- A topic-definition file that describes the SDN topic payload.

## XML Configuration

`oscil-nds-sdn-conf.xml` is the driver configuration file. It declares the SDN topic, which must be defined in the topic-definition file, and maps each topic attribute to a source NDS PV. Set `trigger_pub` to `yes` when an update to that PV must publish the topic immediately.

```xml
<?xml version="1.0"?>
<nds-sdn>
	<topic name="oscil-nds-sdn-topic">
		<attribute name="NumberOfPushedDataBlocks_Ch0">
			<nds-pv name="B-7-DataAcquisitionNode_Ch0-NumberOfPushedDataBlocks" type="int32" />
			<trigger_pub>yes</trigger_pub>
		</attribute>
		<attribute name="NumberOfPushedDataBlocks_Ch1">
			<nds-pv name="B-7-DataAcquisitionNode_Ch1-NumberOfPushedDataBlocks" type="int32" />
			<trigger_pub>yes</trigger_pub>
		</attribute>
	</topic>
</nds-sdn>
```

`oscil-nds-sdn-topic.xml` is the SDN topic-definition file. It specifies the payload, including each attribute's name, rank, and SDN data type. Its `name` must match the topic name in `oscil-nds-sdn-conf.xml`; the driver uses that name to find the file.

```xml
<topic name="oscil-nds-sdn-topic" version="1.0" description="SDN variables for oscilloscope mode">
  <attribute name="NumberOfPushedDataBlocks_Ch0" dataType="int32" description="NoB for channel 0" unit="" qualifier="" rank="1"/>
  <attribute name="NumberOfPushedDataBlocks_Ch1" dataType="int32" description="NoB for channel 1" unit="" qualifier="" rank="2"/>
</topic>

```

The topic-definition filename must exactly match the topic name: `<topic_name>.xml`.

## SDN Archiver Configuration
To allow the SDN Archiver to store the topic in an HDF5 archive, copy the topic-definition XML file (`<topic_name>.xml`) to the SDN configuration directory on the archiver host. This directory is normally set by `SDN_CONFIG_DIRECTORY`; its default location is `${CODAC_CONF}/sdn` or `/etc/opt/codac/sdn`.

On both the publisher host and the host running `sdn-archiver`, `SDN_INTERFACE_NAME` must identify the physical interface used for SDN traffic. For example, use `eth0` rather than the loopback interface `lo`. Configure it with:

```bash
codac-configure sdn_interface eth0
export SDN_INTERFACE_NAME=eth0
```

By default, `sdn-archiver` stores data in `/tmp/sdn-record/`.

Start the `sdn-archiver` service:

```bash
sdn-archiver-service start
```

Verify that `SDN_INTERFACE_NAME` has not changed after the service starts.

Start SDN discovery:

```bash
sdn-discover start
```

Both services write to `ccslog-local.log`:

```bash
tail -f "${LOG_FILE_PATH}/ccslog-local.log"
```

Start archiving the topic:

```bash
sdn-archiver -t oscil-nds-sdn-topic
```

Ensure that the archiver can write to the output directory:

```bash
chmod 777 /tmp/sdn-record
```

View an HDF5 archive file:

```bash
QT_QPA_PLATFORM=xcb sdn-plot VN  oscil_nds_sdn_topic_sdn_archiver_1789645670492539774.h5   oscil-nds-sdn-topic.NumberOfPushedDataBlocks_Ch0,oscil-nds-sdn-topic.NumberOfPushedDataBlocks_Ch1
```

Inspect sent and received packets:

```bash
sudo tcpdump -i enp37s0f0 port 29542
```

## IOC Driver Configuration
Configure and create the driver instance in `userPreDriverConf.cmd`. The `ndsCreateDevice` command specifies the SDN network interface, topic name, and driver-mapping XML file:

```bash
ndsCreateDevice(DeviceNDSSDNPUB, "SDN-0","INTERFACE=lo","TOPICNAME=oscil-nds-sdn-topic","PVS_CONFIG_PATH=../resources/oscil-nds-sdn-conf.xml")
```

Load the associated EPICS DB or substitutions file to create the driver's state-machine and control-PV records.

You can create as many NDS-SDN driver instances as the application requires. Each instance publishes one SDN topic.


## Dependencies

- NDS3 library (provided by CODAC Core System)
- SDN library (provided by CODAC Core System)
- libxml2 library
- C++11 or later compiler

## Authors

- Alexandr Chernikov

## License

This project is developed within the framework of the ITER Organization and follows the internal software distribution policies of the ITER CODAC System.