## nds-sdn-oscil

NDS3-SDN driver for oscilloscope mode

## Description

This driver provides NDS3-SDN (Nominal Device Support v3 Synchronous Data bus Network) device support for transmitting Number of pushed data blocks varaiable of oscilloscope mode. It enables SDN transmitting and integration with the CODAC Core System for the VNC (Vertical Neutron Camera) data acquisition system.

## Features

- **State Machine Management**: Full state machine implementation (OFF → ON → RUNNING) with proper state transitions
- **Data Transimitting**: Data transmitting from nds-oscilloscope with SDN
- **EPICS Integration**: Seamless integration with EPICS IOCs through NDS3-EPICS interface

## Hardware/software Requirements

- nds-oscilloscope driver
- CODAC Core System 7.4.0 or later

## Building

```bash
cd nds-sdn-oscil
make
```

The build process will create the shared library:
- `target/lib/libnds3-sdn-oscil.so`

## Usage

### Loading the Driver in EPICS IOC

Add the following to your `userPreDriverConf.cmd`:

```bash
ndsLoadDriver $(PWD)/../c++/nds-sdn-oscil/lib/libnds3-sdn-oscil.so
ndsCreateDevice(DeviceNDSSDNPUB, "SDN-0","INTERFACE=lo","TOPICNAME=topic-nds-oscil","PVS_CONFIG_PATH=../resources/input.xml")
```

### Device Initialization

The device can be initialized in a single state:

```bash
# Start in OFF state (default)
ndsCreateDevice(DeviceNDSSDNPUB, "SDN-0","INTERFACE=lo","TOPICNAME=topic-nds-oscil","PVS_CONFIG_PATH=../resources/input.xml")
```

## Driver Structure (Nodes and Process Variables)

The driver exposes a single **Port** (root node) named by the device instance (e.g. `SDN-0`). Under it is one **StateMachine** node, and a root-level Process Variable.

Full PV path format: `{DeviceName}-{NodeName}-{PVName}` (e.g. `SDN-0-StateMachine-getState`, `SDN-0-publishingRate`).

---

### 1. StateMachine (Device state machine)

NDS3 standard state machine for the whole device (OFF → ON → RUNNING).

| PV Name     | Direction | Type   | Description                   |
|------------|-----------|--------|--------------------------------|
| `getState` | In        | int32  | Current state (read-only)      |
| `setState` | Out       | int32  | Command to change state (write)|

---

### 2. Root-level Process Variable (direct children of Port)

| PV Name             | Type  | Description                             |
|---------------------|-------|-----------------------------------------|
| `publishingRate`    | int32 | Publishing rate (us) for the SDN Topic  |

---

### 4. Structure overview (tree)

```
{DeviceName}  (Port - root)
├── StateMachine
│   ├── getState      (In,  int32)
│   └── setState      (Out, int32)
└── [Root-level PVs - see tables above]
```

### 5. Threads and data flow

- **Device StateMachine**: `switchOn_Device`, `switchOff_Device`, `start_Device`, `stop_Device`, `recover_Device` are implemented (currently empty); transitions are allowed.
- **Main thread**:
  - **PV_int32Attribute_writer** function: receives data from NDS PV and pushes it to SDN.

## Testing

...

## Dependencies

- NDS3 library (provided by CODAC Core System)
- SDN library (provided by CODAC Core System)
- libxml2 library
- C++11 or later compiler

## Authors

- Alexandr Chernikov

## License

This project is developed within the framework of the ITER Organization and follows the internal software distribution policies of the ITER CODAC System.