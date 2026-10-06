#include "./../include/OscilProcessing.h"

#define NDS_EPOCH 1514764800

//DataAcquisitionNode StateMachine for channel 0
void OscilProcessing::switchOn_DataAcquisition_Ch0() {}
void OscilProcessing::switchOff_DataAcquisition_Ch0() {}
void OscilProcessing::recover_DataAcquisition_Ch0() {}

void OscilProcessing::start_DataAcquisition_Ch0() {
  m_bStop_FIFO_Ch0_DataAcquisition.store(false, std::memory_order_relaxed); //starts the cycle
  m_FIFO_Ch0_DataAcquisition_Thread =
      m_DataAcquisition_Ch0.runInThread("FIFO_CH0_DAQ", 
          std::bind( & OscilProcessing::FIFO_Ch0_DataAcquisition_thread_body, this)); // runs the thread in thread body method
}

void OscilProcessing::stop_DataAcquisition_Ch0() {
  m_bStop_FIFO_Ch0_DataAcquisition.store(true, std::memory_order_relaxed);
  m_FIFO_Ch0_DataAcquisition_Thread.join();
}

bool OscilProcessing::allow_DataAcquisition_Change_Ch0(const nds::state_t,
    const nds::state_t, 
    const nds::state_t) {
        return true;
}

//Thread for Channel 0 data from LabVIEW FPGA DMA FIFO
void OscilProcessing::FIFO_Ch0_DataAcquisition_thread_body() {
  NiFpga_Status FIFO_status;
  std::int16_t* pFIFO_Ch0_Data;
  std::vector<int32_t> FIFO_Ch0_Data;
  
  size_t NumberOfPushedDataBlocks_Ch0 = 0;
  size_t elements_requested_Ch0 = 2047;

  size_t FIFO_Ch0_DataAvailable = 0;

  while (!m_bStop_FIFO_Ch0_DataAcquisition.load(std::memory_order_relaxed)) {
    //Acquire all Target-to-Host DMA FIFOs
    //Channel 0 Data
    FIFO_status = NiFpga_AcquireFifoReadElementsI16(
      session,
      NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch0, 
      &pFIFO_Ch0_Data, 0, 0,
      &FIFO_Ch0_DataAcquired, 
      &FIFO_Ch0_DataAvailable);

    if (FIFO_Ch0_DataAvailable > 0) {
      FIFO_status = NiFpga_AcquireFifoReadElementsI16(
        session,
        NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch0, 
        &pFIFO_Ch0_Data, 
        FIFO_Ch0_DataAvailable, 0,
        &FIFO_Ch0_DataAcquired, 
        &FIFO_Ch0_DataAvailable);

      FIFO_Ch0_Data.insert(FIFO_Ch0_Data.end(),  pFIFO_Ch0_Data, pFIFO_Ch0_Data+FIFO_Ch0_DataAcquired);
      
      FIFO_status = NiFpga_ReleaseFifoElements(
        session, 
        NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch0, 
        FIFO_Ch0_DataAcquired);

      m_DataAcquisition_Ch0.push(m_DataAcquisition_Ch0.getTimestamp(), FIFO_Ch0_Data);
      ++NumberOfPushedDataBlocks_Ch0;
    }
    FIFO_Ch0_Data.clear();
  }
  m_DataAcquisition_Ch0.setNumberOfPushedDataBlocks(m_DataAcquisition_Ch0.getTimestamp(), NumberOfPushedDataBlocks_Ch0);
}


/*Channel 0*/
void OscilProcessing::PV_DataAcquisition_Gain_Writer_Ch0(const timespec & timestamp,
  const double & value) {
  double HW_value;
  HW_value = value;
  m_DataAcquisition_Ch0.setGain(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Offset_Writer_Ch0(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Offset to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Offset programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setOffset(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Bandwidth_Writer_Ch0(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Bandwidth to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Bandwidth programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setBandwidth(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Resolution_Writer_Ch0(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Resolution to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Resolution programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setResolution(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Impedance_Writer_Ch0(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Impedance to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Impedance programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setImpedance(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Coupling_Writer_Ch0(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the Coupling to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Coupling programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setCoupling(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_SignalRefType_Writer_Ch0(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the SignalRefType to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real SignalRefType programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setSignalRefType(timestamp, HW_value);
}
void OscilProcessing::PV_DataAcquisition_Ground_Writer_Ch0(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the Ground to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Ground programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setGround(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_DMAEnable_Writer_Ch0(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the DMAEnable value to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real DMAEnable value programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setDMAEnable(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_SamplingRate_Writer_Ch0(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the SamplingRate to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real SamplingRate programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setSamplingRate(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Gain_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    10
  };
  * value = 2.5; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Offset_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    20
  };
  * value = 3.0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Bandwidth_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    30
  };
  * value = 10.0; //Note that this value has no sense and it is fixed only for testing purposes.

}

void OscilProcessing::PV_DataAcquisition_Resolution_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    40
  };
  * value = 15.0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Impedance_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    50
  };
  * value = 2.2; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Coupling_Initializer_Ch0(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    60
  };
  * value = 1; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_SignalRefType_Initializer_Ch0(
  timespec * timestamp, int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    70
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Ground_Initializer_Ch0(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    80
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_DMAEnable_Initializer_Ch0(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    90
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_SamplingRate_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    100
  };
  * value = 10.0; //Note that this value has no sense and it is fixed only for testing purposes.
}