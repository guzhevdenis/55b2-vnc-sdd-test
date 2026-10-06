## OscilDANCP

NDS3-DAN driver for oscilloscope mode

## Description

This driver provides NDS3-DAN (Nominal Device Support v3-Data Archiving Network) device support for archiving data of oscilloscope mode. It enables data archiving and integration with the CODAC Core System for the VNC (Vertical Neutron Camera) data acquisition system.

## Features

- **State Machine Management**: Full state machine implementation (OFF → ON → RUNNING) with proper state transitions
- **Data Archiving**: High-speed data archiving from nds-oscilloscope
- **Process Variables**: Comprehensive set of PVs for device configuration and monitoring
- **EPICS Integration**: Seamless integration with EPICS IOCs through NDS3-EPICS interface

## Hardware/software Requirements

- nds-oscilloscope driver
- CODAC Core System 7.4.0 or later

## Building

```bash
cd DANDriver/OscilDANCP
make
```

The build process will create the shared library:
- `lib/libnds-dan-oscil-device-Copy.so`

## Usage

### Loading the Driver in EPICS IOC

Add the following to your `userPreDriverConf.cmd`:

```bash
ndsLoadDriver $(PWD)/../c++/DANDrivers/OscilDANCP/lib/libnds-dan-oscil-device-Copy.so
ndsCreateDevice(OscilDANCP, DAN-1, "INIT=YES")

nds subscribe DAN-1-buffer_Ch0 B-7-DataAcquisitionNode_Ch0-Data
nds subscribe DAN-1-buffer_Ch1 B-7-DataAcquisitionNode_Ch1-Data
```

### Device Initialization

The device can be initialized in a single state:

```bash
# Start in OFF state (default)
ndsCreateDevice(OscilDANCP, DAN-1, "INIT=YES")
```

### Subscription to NDS PV

It is mandatory to add NDS PV from which the data will be copied

```bash
nds subscribe DAN-1-buffer_Ch0 B-7-DataAcquisitionNode_Ch0-Data
nds subscribe DAN-1-buffer_Ch1 B-7-DataAcquisitionNode_Ch1-Data
```

## Driver Structure (Nodes and Process Variables)

The driver exposes a single **Port** (root node) named by the device instance (e.g. `DAN-1`). Under it are one **StateMachine** node, two **DAN** nodes and a set of root-level Process Variables.

Full PV path format: `{DeviceName}-{NodeName}-{PVName}` for State Machine (e.g. `DAN-1-StateMachine_Ch0-getState`), `{DeviceName}-{NodeName}-{DANName}-{PVName}` for other PVs (e.g. `DAN-1-DAN_Ch0-Status`).

---

### 1. StateMachine

### 1.1 StateMachine_Ch0 (State machine for DAN_Ch0 node)

NDS3 standard state machine for the whole device (OFF → ON → RUNNING).

| PV Name     | Direction | Type   | Description                   |
|------------|-----------|--------|--------------------------------|
| `getState` | In        | int32  | Current state (read-only)      |
| `setState` | Out       | int32  | Command to change state (write)|

---

### 2. DAN nodes (child nodes)

NDS3-DAN nodes named `DAN_Ch0` and `DAN_Ch1`. Each node has its own state machine and standard DAN PVs.

#### 2.1 DAN_Ch0

#### 2.1.1 StateMachine_Ch0 (State machine for DAN_Ch0 node)

NDS3 standard state machine for the DAN_Ch0 node (OFF → ON → RUNNING).

| PV Name     | Direction | Type   | Description                   |
|------------|-----------|--------|--------------------------------|
| `getState` | In        | int32  | Current state (read-only)      |
| `setState` | Out       | int32  | Command to change state (write)|

#### 2.1.2 DAN_Ch0 configuration PVs (read/write)

| PV Name               | Type   | Description                           |
|-----------------------|--------|---------------------------------------|
| `Status`              | int32  | DAN status                            |
| `StatusStream`        | int32  | DAN Stream status                     |
| `BlockNumberOfSamples`| int32  | DAN block size                        |
| `BufferSize`          | int32  | DAN buffer size                       |
| `ICProgramName`       | char   | Name of DAN ICProgram                 |
| `NumberOfChannels`    | int32  | Number of channels in DAN streamer    |
| `PathXML`             | char   | Path of DAN XML file                  |
| `SampleSize`          | int32  | DAN sample size                       |
| `SamplingRate`        | int32  | DAN sampling rate                     |
| `SourceName`          | char   | DAN source name                       |
| `MaxAcquisitionTime`  | uint64 | Maximum acquisition time              |
| `NumberOfBlocks`      | int32  | Number of data blocs in DAN buffer    |
| `TimeBetweenBlocks`   | uint64 | Time between consecutive data blocks  |
 
#### 2.2 DAN_Ch1

#### 2.2.1 StateMachine_Ch1 (State machine for DAN_Ch1 node)

NDS3 standard state machine for the DAN_Ch1 node (OFF → ON → RUNNING).

| PV Name     | Direction | Type   | Description                   |
|------------|-----------|--------|--------------------------------|
| `getState` | In        | int32  | Current state (read-only)      |
| `setState` | Out       | int32  | Command to change state (write)|

#### 2.2.2 DAN_Ch1 configuration PVs (read/write)

| PV Name               | Type   | Description                           |
|-----------------------|--------|---------------------------------------|
| `Status`              | int32  | DAN status                            |
| `StatusStream`        | int32  | DAN Stream status                     |
| `BlockNumberOfSamples`| int32  | DAN block size                        |
| `BufferSize`          | int32  | DAN buffer size                       |
| `ICProgramName`       | char   | Name of DAN ICProgram                 |
| `NumberOfChannels`    | int32  | Number of channels in DAN streamer    |
| `PathXML`             | char   | Path of DAN XML file                  |
| `SampleSize`          | int32  | DAN sample size                       |
| `SamplingRate`        | int32  | DAN sampling rate                     |
| `SourceName`          | char   | DAN source name                       |
| `MaxAcquisitionTime`  | uint64 | Maximum acquisition time              |
| `NumberOfBlocks`      | int32  | Number of data blocs in DAN buffer    |
| `TimeBetweenBlocks`   | uint64 | Time between consecutive data blocks  |
 
---

### 3. Structure overview (tree)

```
{DeviceName}  (Port - root)
├── StateMachine
│   ├── getState      (In,  int32)
│   └── setState      (Out, int32)
├── DAN_Ch0
│   ├── StateMachine
│   │   ├── getState  (In,  int32)
│   │   └── setState  (Out, int32)
│   ├── Status, StatusStream, BlockNumberOfSamples, BufferSize,
│   ├── Status, StatusStream, BlockNumberOfSamples, BufferSize,
│   ├── ICProgramName, NumberOfChannels, PathXML, SampleSize,
│   ├── SamplingRate, SourceName, MaxAcquisitionTime,
│   └── NumberOfBlocks, TimeBetweenBlocks
│
└── DAN_Ch1
    ├── StateMachine
    │   ├── getState  (In,  int32)
    │   └── setState  (Out, int32)
    ├── Status, StatusStream, BlockNumberOfSamples, BufferSize,
    ├── ICProgramName, NumberOfChannels, PathXML, SampleSize,
    ├── SamplingRate, SourceName, MaxAcquisitionTime,
    └── NumberOfBlocks, TimeBetweenBlocks

```

---

### 4. Threads and data flow

- **Nodes StateMachine**: `switchOn_ch0/1`, `switchOff_ch0/1`, `start_ch0/1`, `stop_ch0/1`, `recover_ch0/1` are implemented; transitions are allowed.
- **Main thread**:
  - **PV_buffer_writer_Ch0** function: receives data from NDS PV for channel 0; pushes to DAN with 50 milliseconds latency.
  - **PV_buffer_writer_Ch1** function: receives data from NDS PV for channel 1; pushes to DAN with 50 milliseconds latency.

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