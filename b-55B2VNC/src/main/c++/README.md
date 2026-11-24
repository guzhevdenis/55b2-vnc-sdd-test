- AlgorithmsTesting - C++ applications used to validate functionality implemented in the NDS drivers.
- DANDataGenerator - Test NDS driver used to generate data archived by the nds-dan driver.
- DANDrivers - NDS-DAN drivers.
- DANDrivers/OscilDANCP - NDS-DAN driver for the nds-oscilloscope implementation. Configuration for DAN is located in `DANDrivers/OscilDANCP/conf`.
- DataAgregator - Test NDS driver used to aggregate count-rate data produced by the data generator and calculate measurement parameters with an ML model.
- DataGenerator - Test NDS driver used to generate count-rate data.
- nds-dan - Source files used to build the nds-dan library.
- nds-oscilloscope - NDS driver for oscilloscope mode.
- nds-sdn-oscil - SDN publishing/driver support for the nds-oscilloscope implementation. Configuration for SDN is located in `src/main/resources`.
- nds3-6683h - NDS driver support for the 6683H board.
- nds3-ni5772 - NDS driver support for the NI5772/7976R board.
- nds3-receiver - Test driver for receiving count-rate (temperature) data from other PCFs.
- nds3-sdn - Source files for NDS/SDN interface support.
- nds3-sender - Test driver for sending count-rate (temperature) data to other PCFs.
- onnxruntime-linux-x64-1.21.0 - ML runtime library used by the project.

Note: the receiver and sender drivers are intended to be tested on different machines.