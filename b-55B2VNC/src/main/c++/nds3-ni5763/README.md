## nds3-ni5763

NDS3 driver for National Instruments NI-5763 ADC module with PXIe-7976R FPGA board.

## Description

This driver provides NDS3 (Nominal Device Support v3) device support for the National Instruments NI-5763 module combined with the PXIe-7976R FPGA board. It enables real-time data acquisition, signal processing, and integration with the CODAC Core System for the VNC (Vertical Neutron Camera) data acquisition system.
## Features

- **State Machine Management**: Full state machine implementation (OFF → ON → RUNNING) with proper state transitions
- **Data Acquisition**: High-speed data acquisition from FPGA with configurable parameters
- **Process Variables**: Comprehensive set of PVs for device configuration and monitoring
- **EPICS Integration**: Seamless integration with EPICS IOCs through NDS3-EPICS interface

## Hardware/software Requirements

- National Instruments PXIe-7976R FPGA board
- National Instruments NI-5763 ADC module
- CODAC Core System 7.4.0 or later
- NI FPGA Runtime and drivers

## Building

```bash
cd nds3-ni5763
make
```

The build process will create the shared library:
- `target/lib/libnds3-Ni5763.so`

## Usage

### Loading the Driver in EPICS IOC

Add the following to your `userPreDriverConf.cmd`:

```bash
ndsLoadDriver $(PWD)/../c++/nds3-ni5763/target/lib/libnds3-Ni5763.so
ndsCreateDevice Ni5763 B-8
```

### Device Initialization

The device can be initialized in a single state:

```bash
# Start in OFF state (default)
ndsCreateDevice Ni5763 B-8
```

### Configuring FIFO depth

FIFO depth should be configured before switching on the State Machine. If its not done the default depth of 10240 will be set.

## Driver Structure (Nodes and Process Variables)

The driver exposes a single **Port** (root node) named by the device instance (e.g. `B-8`). Under it are one **StateMachine** node, one **DataAcquisitionNode**, and a set of root-level Process Variables.

Full PV path format: `{DeviceName}-{NodeName}-{PVName}` (e.g. `B-8-StateMachine-getState`, `B-8-DataAcquisitionNode_FissCh-Data`).

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

| PV Name                        | Type          | Description                                        |
|--------------------------------|---------------|----------------------------------------------------|
| `Status`                       | int32         | Status                                             |
| `DAQ_On`                       | int32 (Bool)  | Shows if Data Acquisition turned on                |
| `Input_Valid`                  | int32 (Bool)  | Validness of data input                            |
| `ReadyForInput`                | int32 (Bool)  | Shows if data is ready for input                   |
| `EEPROMPowerEnabled`           | int32 (Bool)  | Shows EEPROM state                                 |
| `Expected_ID`                  | int64 (U64)   | Expected I/O module ID                             |
| `Inserted_ID`                  | int64 (U64)   | Actual I/O module ID                               |
| `IOEnabled`                    | int32 (Bool)  | I/O module enabled                                 |
| `PowerEnabled`                 | int32 (Bool)  | I/O module power enabled                           |
| `PowerGood`                    | int32 (Bool)  | I/O module power good                              |
| `Present`                      | int32 (Bool)  | I/O module present                                 |
| `InitializationDone`           | int32 (Bool)  | I/O module init done                               |
| `Clk100PLL_Unlocked`           | int32 (Bool)  | Lock loss on PLL frequency of 100 MHz              |
| `Power3_3V`                    | float64 (U16) | Device power in 3.3V line                          |
| `Power12V`                     | float64 (U16) | Device power in 12V line                           |
| `RawDataFIFO_Depth`            | int32 (I32)   | Actual depth of Raw data FIFO                      |
| `FluctCountRate_Hz_FIFO_Depth` | int32 (I32)   | Actual depth of FluctCountRate_Hz FIFO             |
| `PulseChainFIFO_Depth`         | int32 (I32)   | Actual depth of PulseChain FIFO                    |

  
#### 2.2 Controls (write)  
| PV Name                      | Type                 | Description                            |
|------------------------------|----------------------|----------------------------------------|
| `ConfigFIFO_Depth`           | int32                | Set FIFO depth                         |
| `EnableDAQ`                  | int32 (Bool)         | On/off Data Acquisition                |
| `FirstExpCoeff`              | int32 (I16)          | Set 1st exponential filter coefficient |
| `FirstExpCoeff`              | int32 (I16)          | Set 2nd exponential filter coefficient |
| `Type`                       | int32 (U16)          | Set type of filtration                 |
| `CalibrationCoefficient`     | float64 (U64)        | Set calibration coefficient            |
| `Threshold`                  | float64 (I16)        | Set threshold                          |
| `SinglePulseMax`             | int32 (U16)          | Set maximum single pulses              |
| `SinglePulseMin`             | int32 (U16)          | Set minimum single pulses              |
| `TotalPulseMin`              | int32 (U16)          | Set minimum total pulses               |
| `FluctCountRate_Hz`          | float64              | Fluctuation count rate                 |
| `SinglePules`                | int32                | Single pulses                          |
| `SingleRate`                 | float64              | Single rate                            |
| `TotalPulses`                | int32                | Total pulses                           |
| `TotalRate`                  | float64              | Total rate                             |
| `SingleRateData`             | vector&lt;double&gt; | Single rate PV for display on graph    |
| `TotalRateData`              | vector&lt;double&gt; | Total rate PV for display on graph     |

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

- **Device StateMachine**: `switchOn_Device`, `switchOff_Device` are implemented, `start_Device`, `stop_Device`, `recover_Device` also implemented (currently empty); transitions are allowed.
- **DataAcquisitionNode**:
  - **RAW_DAQ_CH0** thread: reads RawData FIFO (I16) and pushes raw blocks to DataAcquisition.
  - **DAQ_CH0** thread: reads FluctCountRate_Hz_ToHost FIFO (U32) PulseChain FIFO (U32); updates `FluctCountRate_Hz`, `SinglePules`, `SingleRate`, `TotalPulses`, `TotalRate`, 
                                                                                                 `SingleRateData` and `TotalRateData`; pushes to DataAcquisition_Ch1.
- Root-level PVs are read from or written to the FPGA via NiFpga API (indicators/controls);

## FPGA Bitfiles

The driver uses LabVIEW FPGA bitfiles located in the `bitfile/` directory. The default bitfile is `NiFpga_Fission_Chamber_DAQ_FPGA_Top.lvbitx`.

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