## nds-dan-plugin

NDS3-DAN-Plugin for NI5763

## Description

This plugin provides NDS3-DAN (Nominal Device Support v3-Data Archiving Network) device support for archiving raw data from NI5763. It enables data archiving and integration with the CODAC Core System for the VNC (Vertical Neutron Camera) data acquisition system.

## Features

- **State Machine Management**: Full state machine implementation (OFF → ON → RUNNING) with proper state transitions
- **Data Archiving**: High-speed data archiving from NI5763
- **EPICS Integration**: Seamless integration with EPICS IOCs through NDS3-EPICS interface

## Hardware/software Requirements

- NI5763 driver
- CODAC Core System 7.4.0 or later

## Building

```bash
cd DANDriver/nds-dan-plugin
make
```

The build process will create the shared library:
- `lib/libnds3-dev-nds-dan-plugin.so`

## Usage

### Loading the Driver in EPICS IOC

Add the following to your `userPreDriverConf.cmd`:

```bash
ndsLoadDriver $(PWD)/../c++/DANDrivers/nds-dan-plugin/lib/libnds3-dev-nds-dan-plugin.so
ndsCreateDevice(PluginNDSDAN, "NDSDAN-0", "DAN_PLUGIN_XML=../resources/ni5763-nds-dan-metadata.xml", "DAN=YES")

```

In `../resources/ni5763-dan-config.xml` value `nelm` must be equal to FIFO size.

## Driver Structure (Nodes and Process Variables)

The driver exposes a single **Port** (root node) named by the device instance (e.g. `NDSDAN-0`). Under it are one **StateMachine** node, one **DAN** node and a set of root-level Process Variables.

Full PV path format: `{DeviceName}-{NodeName}-{PVName}` for State Machine (e.g. `NDSDAN-0-StateMachine-setState-getState`).

---

### 1. StateMachine

### 1.1 StateMachine_Ch0 (State machine for DAN_Ch0 node)

NDS3 standard state machine for the whole device (OFF → ON → RUNNING).

| PV Name     | Direction | Type   | Description                   |
|------------|-----------|--------|--------------------------------|
| `getState` | In        | int32  | Current state (read-only)      |
| `setState` | Out       | int32  | Command to change state (write)|

---

### 4. Threads and data flow

- **Node StateMachine**: `switchOn_Device`, `switchOff_Device`, `start_Device`, `stop_Device`, `recover_cDevice` are implemented; transitions are allowed.

## Testing

...

## Dependencies

- NDS3 library (provided by CODAC Core System)
- NDS_DAN library
- C++11 or later compiler

## Authors

- Alexandr Chernikov

## License

This project is developed within the framework of the ITER Organization and follows the internal software distribution policies of the ITER CODAC System.