## nds-oscilloscope

NDS3 driver for oscilloscope mode

## Description

This driver provides NDS3 (Nominal Device Support v3) device support for oscilloscope mode. It enables real-time data acquisition and integration with the CODAC Core System for the VNC (Vertical Neutron Camera) data acquisition system.

## Features

- **State Machine Management**: Full state machine implementation (OFF → ON → RUNNING) with proper state transitions
- **Data Acquisition**: High-speed data acquisition from FPGA with configurable parameters
- **Process Variables**: Comprehensive set of PVs for device configuration and monitoring
- **EPICS Integration**: Seamless integration with EPICS IOCs through NDS3-EPICS interface

## Hardware/software Requirements

- FPGA board
- CODAC Core System 7.4.0 or later
- NI FPGA Runtime and drivers

## Building

```bash
cd nds-oscilloscope
make
```

The build process will create the shared library:
- `target/lib/libnds3-OscilProcessing.so`

## Usage

### Loading the Driver in EPICS IOC

Add the following to your `userPreDriverConf.cmd`:

```bash
ndsLoadDriver $(PWD)/../c++/nds-oscilloscope/target/lib/libnds3-OscilProcessing.so
ndsCreateDevice OscilProcessing B-7
```

### Device Initialization

The device can be initialized in a single state:

```bash
# Start in OFF state (default)
ndsCreateDevice OscilProcessing B-7
```

## Driver Structure (Nodes and Process Variables)

The driver exposes a single **Port** (root node) named by the device instance (e.g. `B-7`). Under it are one **StateMachine** node, two **DataAcquisitionNode**, and a set of root-level Process Variables.

Full PV path format: `{DeviceName}-{NodeName}-{PVName}` (e.g. `B-7-StateMachine-getState`, `B-7-DataAcquisitionNode_Ch0-Data`).

---

### 1. StateMachine (Device state machine)

NDS3 standard state machine for the whole device (OFF → ON → RUNNING).

| PV Name     | Direction | Type   | Description                   |
|------------|-----------|--------|--------------------------------|
| `getState` | In        | int32  | Current state (read-only)      |
| `setState` | Out       | int32  | Command to change state (write)|

---

### 2. Root-level Process Variables (direct children of Port)

#### 2.1 Indicators (read-only, from FPGA or software)

| PV Name                      | Type          | Description                                        |
|------------------------------|---------------|----------------------------------------------------|
| `IOModuleInitializationDone` | int32 (Bool)  | I/O module init done                               |
| `DAQ_On`                     | int32 (Bool)  | Shows if Data Acquisition turned on                |
| `Rec_Ch0`                    | int32 (Bool)  | FIFO queue activity flag for channel 0             |
| `Rec_Ch1`                    | int32 (Bool)  | FIFO queue activity flag for channel 1             |
| `Elements_Ch0`               | int32 (I16)   | Last 4 readouts from channel 0                     |
| `Elements_Ch1`               | int32 (I16)   | Last 4 readouts from channel 1                     |
| `SizeElements_Ch0`           | int32 (I16)   | Size of Elements_Ch0                               |
| `SizeElements_Ch1`           | int32 (I16)   | Size of Elements_Ch1                               |

  
#### 2.2 Controls (write)  
| PV Name                      | Type          | Description                                        |
|------------------------------|---------------|----------------------------------------------------|
| `DAQ_En   `                  | int32 (Bool)  | On/off Data Acquisition                            |
| `Channel_0`                  | int32 (Bool)  | On/off channel 0                                   |
| `FSA_On_Ch0`                 | int32 (Bool)  | On/off front search algorythm for channel 0        |
| `Channel_1`                  | int32 (Bool)  | On/off channel 1                                   |
| `FSA_On_Ch1`                 | int32 (Bool)  | On/off front search algorythm for channel 1        |
| `Reference_Ch0`              | int32 (I16)   | Condition for recording the pulse signal channel 0 |
| `Reference_Ch1`              | int32 (I16)   | Condition for recording the pulse signal channel 1 |
| `Delta_Ch0`                  | int32 (I16)   | Min values diff for confirmation of front existing |
| `Delta_Ch1`                  | int32 (I16)   | Min values diff for confirmation of front existing |
| `Difference_Ch0`             | int32 (I16)   | Condition for recording the pulse signal channel 0 |
| `Difference_Ch1`             | int32 (I16)   | Condition for recording the pulse signal channel 1 |

---

### 3. DataAcquisitionNode (child node)

NDS3 **DataAcquisition** nodes named `DataAcquisitionNode_Ch0` and `DataAcquisitionNode_Ch1`. Each node has its own state machine and standard data-acquisition PVs. Data type: `vector<int32_t>`, buffer size 800000.

#### 3.1 DataAcquisitionNode_Ch0

#### 3.1.1 DataAcquisitionNode state machine

| PV Name     | Direction | Type  | Description              |
|-------------|-----------|-------|--------------------------|
| `getState`  | In        | int32 | Current node state       |
| `setState`  | Out       | int32 | Command to change state  |

#### 3.1.2 DataAcquisitionNode configuration PVs (read/write) /*curruntly not used*/

| PV Name         | Type   | Description                    |
|-----------------|--------|--------------------------------|
| `Gain`          | double | Gain                           |
| `Offset`        | double | Offset                         |
| `Bandwidth`     | double | Bandwidth                      |
| `Resolution`    | double | Resolution                     |
| `Impedance`     | double | Impedance                      |
| `Coupling`      | int32  | Coupling                       |
| `SignalRefType` | int32  | Signal reference type          |
| `Ground`        | int32  | Ground reference               |
| `DMAEnable`     | int32  | DMA enable                     |
| `SamplingRate`  | double | Sampling rate                  |

#### 3.1.3 DataAcquisitionNode data PVs

| PV Name                    | Type                  | Description                            |
|----------------------------|-----------------------|----------------------------------------|
| `Data`                     | vector&lt;int32_t&gt; | Acquired data stream                   |
| `NumberOfPushedDataBlocks` | (internal)            | Number of pushed data blocks           |

#### 3.2 DataAcquisitionNode_Ch0

#### 3.2.1 DataAcquisitionNode state machine

| PV Name     | Direction | Type  | Description              |
|-------------|-----------|-------|--------------------------|
| `getState`  | In        | int32 | Current node state       |
| `setState`  | Out       | int32 | Command to change state  |

#### 3.2.2 DataAcquisitionNode configuration PVs (read/write) /*curruntly not used*/

| PV Name         | Type   | Description                    |
|-----------------|--------|--------------------------------|
| `Gain`          | double | Gain                           |
| `Offset`        | double | Offset                         |
| `Bandwidth`     | double | Bandwidth                      |
| `Resolution`    | double | Resolution                     |
| `Impedance`     | double | Impedance                      |
| `Coupling`      | int32  | Coupling                       |
| `SignalRefType` | int32  | Signal reference type          |
| `Ground`        | int32  | Ground reference               |
| `DMAEnable`     | int32  | DMA enable                     |
| `SamplingRate`  | double | Sampling rate                  |

#### 3.2.3 DataAcquisitionNode data PVs

| PV Name                    | Type                  | Description                            |
|----------------------------|-----------------------|----------------------------------------|
| `Data`                     | vector&lt;int32_t&gt; | Acquired data stream                   |
| `NumberOfPushedDataBlocks` | (internal)            | Number of pushed data blocks           |

---

### 4. Structure overview (tree)

```
{DeviceName}  (Port - root)
├── StateMachine
│   ├── getState      (In,  int32)
│   └── setState      (Out, int32)
├── [Root-level PVs - see tables above]
├── DataAcquisitionNode_Ch0
│   ├── StateMachine
│   │   ├── getState  (In,  int32)
│   │   └── setState  (Out, int32)
│   ├── Gain, Offset, Bandwidth, Resolution, Impedance
│   ├── Coupling, SignalRefType, Ground, DMAEnable, SamplingRate
│   ├── Data
│   └── NumberOfPushedDataBlocks
└── DataAcquisitionNode_Ch1
    ├── StateMachine
    │   ├── getState  (In,  int32)
    │   └── setState  (Out, int32)
    ├── Gain, Offset, Bandwidth, Resolution, Impedance
    ├── Coupling, SignalRefType, Ground, DMAEnable, SamplingRate
    ├── Data
    └── NumberOfPushedDataBlocks
```

---

### 5. Threads and data flow

- **Device StateMachine**: `switchOn_Device`, `switchOff_Device` are implemented, `start_Device`, `stop_Device`, `recover_Device` also implemented (currently empty); transitions are allowed.
- **DataAcquisitionNode**:
  - **FIFO_CH0_DAQ** thread: reads FIFO data from channel 0; pushes to DataAcquisition_Ch0.
  - **FIFO_CH1_DAQ** thread: reads FIFO data from channel 1; pushes to DataAcquisition_Ch1.
- Root-level PVs are read from or written to the FPGA via NiFpga API (indicators/controls);

## FPGA Bitfiles

The driver uses LabVIEW FPGA bitfiles located in the `bitfile/` directory. The default bitfile is `NiFpga_NI5772_Oscilloscope_FPGA_Top.lvbitx`.

## Testing

...

## Dependencies

- NDS3 library (provided by CODAC Core System)
- NI FPGA Runtime (`libNiFpga.so`)
- NI FlexRIO library (`libniflexrio.so`)
- C++11 or later compiler

## Authors

- Alexandr Chernikov

## License

This project is developed within the framework of the ITER Organization and follows the internal software distribution policies of the ITER CODAC System.