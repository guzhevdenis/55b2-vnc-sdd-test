# nds3-ni5772

NDS3 driver for National Instruments NI-5772 ADC module with PXIe-7976R FPGA board.

## Description

This driver provides NDS3 (Nominal Device Support v3) device support for the National Instruments NI-5772 digitizer module combined with the PXIe-7976R FPGA board. It enables real-time data acquisition, signal processing, and integration with the CODAC Core System for the VNC (Vertical Neutron Camera) data acquisition system.

## Features

- **State Machine Management**: Full state machine implementation (OFF → ON → RUNNING) with proper state transitions
- **Data Acquisition**: High-speed data acquisition from FPGA with configurable parameters
- **Signal Processing**: Real-time signal processing including histogram generation, amplitude detection, and peak analysis
- **Process Variables**: Comprehensive set of PVs for device configuration and monitoring
- **EPICS Integration**: Seamless integration with EPICS IOCs through NDS3-EPICS interface
- **DAN Support**: Data Archiving Network (DAN) integration for data persistence

## Hardware Requirements

- National Instruments PXIe-7976R FPGA board
- National Instruments NI-5772 ADC module
- CODAC Core System 7.2.1 or later
- NI FPGA Runtime and drivers

## Building

```bash
cd nds3-ni5772
make
```

The build process will create the shared library:
- `target/lib/libnds3-Ni5772Digitizer.so`

## Usage

### Loading the Driver in EPICS IOC

Add the following to your `userPreDriverConf.cmd`:

```bash
ndsLoadDriver $(PWD)/../c++/nds3-ni5772/target/lib/libnds3-Ni5772Digitizer.so
ndsCreateDevice Ni5772Digitizer B-0
```

### Device Initialization

The device can be initialized in different states:

```bash
# Start in OFF state (default)
ndsCreateDevice Ni5772Digitizer B-0

# Start in ON state
ndsCreateDevice Ni5772Digitizer B-0 "INIT=ON"

# Start in RUNNING state
ndsCreateDevice Ni5772Digitizer B-0 "INIT=RUNNING"
```

## Driver Structure (Nodes and Process Variables)

The driver exposes a single **Port** (root node) named by the device instance (e.g. `B-0` or `NI5772-0`). Under it are one **StateMachine** node, one **DataAcquisitionNode**, and a set of root-level Process Variables.

Full PV path format: `{DeviceName}-{NodeName}-{PVName}` (e.g. `B-0-StateMachine.getState`, `B-0-DataAcquisitionNode-Data`).

---

### 1. StateMachine (Device state machine)

NDS3 standard state machine for the whole device (OFF → ON → RUNNING).

| PV Name     | Direction | Type   | Description                    |
|------------|-----------|--------|--------------------------------|
| `getState` | In        | int32  | Current state (read-only)      |
| `setState` | Out       | int32  | Command to change state (write)|

---

### 2. Root-level Process Variables (direct children of Port)

#### 2.1 Indicators (read-only, from FPGA or software)

| PV Name                    | Type           | Description                                      |
|---------------------------|----------------|--------------------------------------------------|
| `Clock100PLLUnlocked`     | int32 (Bool)   | 100 MHz PLL lock status                          |
| `EEPROMPowerEnabled`      | int32 (Bool)   | EEPROM power enabled                             |
| `IOModuleIOEnabled`       | int32 (Bool)   | I/O module I/O enabled                           |
| `IOModuleInitializationDone` | int32 (Bool) | I/O module init done                             |
| `IOModulePowerEnabled`    | int32 (Bool)   | I/O module power enabled                         |
| `IOModulePowerGood`       | int32 (Bool)   | I/O module power good                            |
| `IOModulePresent`         | int32 (Bool)   | I/O module present                               |
| `IndicatorBoolKey`       | int32 (Bool)   | FPGA key indicator                               |
| `DeviceTemperature`      | int32 (I8)     | Device temperature                               |
| `ResultAfterSub`         | int32 (I16)    | Result after subtraction                         |
| `Counter1`                | int32          | FPGA counter 1                                   |
| `Counter2`                | int32          | FPGA counter 2                                   |
| `Counter3`                | int32          | FPGA counter 3                                   |
| `CounterTemp`             | int32          | Temporary counter                                |
| `SinglePulsesCountRate`   | int32          | Single-pulse count rate                           |
| `SinglePulsesRegistered`  | int32          | Single pulses registered                          |
| `TotalCountRate`          | int32          | Total count rate                                 |
| `TotalPulsesRegistered`   | int32          | Total pulses registered                           |
| `Status`                  | int32          | NiFpga status (debug)                            |
| `MaximumAmplitude`       | double         | Max amplitude from processed data                 |
| `NeutronProfile`         | vector&lt;int32&gt; (max 100) | Neutron profile (proxy for HMI)           |
| `CountRateAverage`       | double         | Average count rate                               |
| `MSV`                    | double         | Mean square value (from MSV ADC FIFO)            |

#### 2.2 Controls (write)

| PV Name                 | Type  | Description                          |
|-------------------------|-------|--------------------------------------|
| `ThresholdValid`        | int32 | Enable/validate threshold            |
| `ControlKey`            | int32 | FPGA control key                     |
| `Reseter`               | int32 | Reset FPGA (write 1 to reset)        |
| `Threshold`             | int32 | Detection threshold (I16)             |
| `Alpha16BitFirst`       | int32 | Alpha coefficient high 16 bits       |
| `Alpha16BitSecond`      | int32 | Alpha coefficient low 16 bits        |
| `FilterType`            | int32 | Filter type (U16)                    |
| `MaxTicksPerPulse`      | int64 | Max ticks per pulse (U32)            |
| `MinTicksPerPulse`      | int64 | Min ticks per pulse (U32)            |
| `MinTicksPerPulse2`     | int64 | Min ticks per pulse 2 (U32)          |
| `CalibrationCoefficient`| double | Calibration coefficient (I64 in FPGA)|
| `Step`                  | int32 | Histogram bin step size              |

---

### 3. DataAcquisitionNode (child node)

NDS3 **DataAcquisition** node named `DataAcquisitionNode`. It has its own state machine and standard data-acquisition PVs. Data type: `vector<int16_t>`, buffer size 800000.

#### 3.1 DataAcquisitionNode state machine

| PV Name     | Direction | Type  | Description              |
|------------|-----------|-------|--------------------------|
| `getState` | In        | int32 | Current node state       |
| `setState` | Out       | int32 | Command to change state  |

#### 3.2 DataAcquisitionNode configuration PVs (read/write)

| PV Name         | Type   | Description                    |
|-----------------|--------|--------------------------------|
| `Gain`          | double | Gain                           |
| `Offset`        | double | Offset                         |
| `Bandwidth`     | double | Bandwidth                      |
| `Resolution`   | double | Resolution                      |
| `Impedance`     | double | Impedance                      |
| `Coupling`      | int32  | Coupling                       |
| `SignalRefType`| int32  | Signal reference type          |
| `Ground`        | int32  | Ground reference               |
| `DMAEnable`     | int32  | DMA enable                     |
| `SamplingRate`  | double | Sampling rate                  |

#### 3.3 DataAcquisitionNode data PVs

| PV Name                   | Type                | Description                          |
|---------------------------|---------------------|--------------------------------------|
| `Data`                    | vector&lt;int16_t&gt; | Acquired data stream (Raw + processed) |
| `NumberOfPushedDataBlocks`| (internal)          | Number of pushed data blocks         |

---

### 4. Structure overview (tree)

```
{DeviceName}  (Port - root)
├── StateMachine
│   ├── getState      (In,  int32)
│   └── setState      (Out, int32)
├── [Root-level PVs - see tables above]
└── DataAcquisitionNode
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

- **Device StateMachine**: `switchOn_Device`, `switchOff_Device`, `start_Device`, `stop_Device` are implemented (currently empty); transitions are allowed.
- **DataAcquisitionNode**:
  - **ProcessedDAQ** thread: reads PulsePeaksFIFO (I16), MSV ADC (U64), Countratefluctuation (U64); updates `MaximumAmplitude`, `CountRateAverage`, `MSV`; pushes to DataAcquisition.
  - **RawDAQ** thread: reads RawData FIFO (I16) and pushes raw blocks to DataAcquisition.
- Root-level PVs are read from or written to the FPGA via NiFpga API (indicators/controls); `NeutronProfile` is a software proxy for HMI.

## FPGA Bitfiles

The driver uses LabVIEW FPGA bitfiles located in the `bitfile/` directory. The default bitfile is `NiFpga_exp_test_FIC.lvbitx`. Different bitfiles can be used by modifying the driver source code.

## Testing

Unit tests are available in `b-55B2VNC/src/test/c++/nds3-ni5772Tests/`. See the test README for details on running the test suite.

## Dependencies

- NDS3 library (provided by CODAC Core System)
- NI FPGA Runtime (`libNiFpga.so`)
- NI FlexRIO library (`libniflexrio.so`)
- C++11 or later compiler

## Authors

- Denis Guzhev
- Danila Avdushkin

## License

This project is developed within the framework of the ITER Organization and follows the internal software distribution policies of the ITER CODAC System.
