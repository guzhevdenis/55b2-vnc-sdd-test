# NI5772Digitizer Unit Tests

This directory contains comprehensive unit tests for the NI5772Digitizer NDS driver.

## Test Files

1. **TP_NI5772_0000.cpp** - Basic device creation and state machine transitions
2. **TP_NI5772_0001.cpp** - Process Variable (PV) read/write operations
3. **TP_NI5772_0002.cpp** - Data Acquisition node state machine and operations
4. **TP_NI5772_0003.cpp** - Helper functions and utility methods
5. **TP_NI5772_0004.cpp** - Vector PV operations (Amplitudes, Numbers, NeutronProfile)
6. **main.cpp** - Test entry point

## Prerequisites

- CODAC Core System 7.2.1 or later
- Google Test framework
- NI FPGA Runtime (for hardware tests)
- NDS3 library

## Building Tests

### Using Makefile

```bash
# Set CODAC_ROOT environment variable
export CODAC_ROOT=/path/to/codac

# Build tests
make

# Run tests
make test

# Clean build artifacts
make clean
```

### Manual Compilation

```bash
g++ -std=c++14 -Wall -Wextra -g \
    -I../../../../main/c++/nds3-ni5772/include \
    -I$(CODAC_ROOT)/include/nds3 \
    -L$(CODAC_ROOT)/lib \
    -L../../../../main/c++/nds3-ni5772/target/lib \
    TP_NI5772_0000.cpp TP_NI5772_0001.cpp TP_NI5772_0002.cpp \
    TP_NI5772_0003.cpp TP_NI5772_0004.cpp main.cpp \
    -lnds3 -lgtest -lgtest_main -lpthread -lNiFpga \
    -o ni5772Tests
```

## Running Tests

```bash
./ni5772Tests
```

For verbose output:
```bash
./ni5772Tests --gtest_output=xml:test_results.xml
```

## Test Coverage

### TP_NI5772_0000: Basic Device Operations
- ✅ Device creation
- ✅ State machine transitions (OFF → ON → RUNNING → ON → OFF)
- ✅ State validation at each transition

### TP_NI5772_0001: Process Variables
- ✅ Status PV read
- ✅ Threshold write/read
- ✅ Alpha16BitFirst/Second write
- ✅ FilterType write
- ✅ MaxTicksPerPulse/MinTicksPerPulse write
- ✅ CalibrationCoefficient write
- ✅ Step write
- ✅ Counter PV reads
- ✅ DeviceTemperature read
- ✅ Reseter write

### TP_NI5772_0002: Data Acquisition Node
- ✅ Data Acquisition node state machine
- ✅ Data Acquisition PV writes (Gain, Offset, Bandwidth, etc.)
- ✅ Data acquisition start/stop operations

### TP_NI5772_0003: Helper Functions
- ✅ convertDoubleToInt64 function
- ✅ Edge cases (zero, negative, overflow)
- ✅ Device instance management
- ✅ Duplicate device name handling

### TP_NI5772_0004: Vector Operations
- ✅ Amplitudes vector read
- ✅ Numbers vector read
- ✅ NeutronProfile vector read (100 elements)
- ✅ MaximumAmplitude read
- ✅ CountRateAverage read
- ✅ MSV read

## Test Environment

These tests use the NDS3 Test Control System, which provides a mock control system interface. This allows testing without requiring actual FPGA hardware or EPICS IOC.

**Note:** Some tests may require the FPGA bitfile to be available. Tests will gracefully handle missing hardware dependencies.

## Troubleshooting

### Driver Library Not Found
If you see warnings about the driver library not being found:
1. Ensure the driver is built: `cd ../../../../main/c++/nds3-ni5772 && make`
2. Check that `target/lib/libnds3-Ni5772Digitizer.so` exists
3. Update the driver path in `main.cpp` if using a different build configuration

### Missing Dependencies
- Ensure CODAC_ROOT is set correctly
- Install Google Test: `sudo apt-get install libgtest-dev` (Linux)
- For NI FPGA support, ensure NiFpga library is available

### Test Failures
- Check that the driver library is built and accessible
- Verify CODAC_ROOT points to correct installation
- Some tests may fail if FPGA hardware is not available (this is expected for unit tests)

## Integration with CI/CD

These tests can be integrated into CI/CD pipelines:

```bash
# Run tests and generate XML report
./ni5772Tests --gtest_output=xml:test_results.xml

# Run specific test
./ni5772Tests --gtest_filter=TP_NI5772.0000
```

## Adding New Tests

To add a new test:

1. Create a new file `TP_NI5772_XXXX.cpp` following the naming convention
2. Use the `TEST(TP_NI5772, XXXX)` macro
3. Follow the pattern from existing tests
4. Add the source file to `Makefile` TEST_SOURCES
5. Update this README with test description

Example test structure:
```cpp
TEST(TP_NI5772, XXXX)
{
    std::string testName = "TP_NI5772_XXXX";
    std::string testDescription("Test description");
    
    // Setup
    // Test execution
    // Assertions
    // Cleanup
}
```


