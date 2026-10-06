#include "./../include/OscilProcessing.h"

#define NDS_EPOCH 1514764800

//DataAcquisitionNode StateMachine for channel 1
void OscilProcessing::switchOn_DataAcquisition_Ch1() {}
void OscilProcessing::switchOff_DataAcquisition_Ch1() {}
void OscilProcessing::recover_DataAcquisition_Ch1() {}

void OscilProcessing::start_DataAcquisition_Ch1() {
  m_bStop_FIFO_Ch1_DataAcquisition.store(false, std::memory_order_relaxed);
  m_FIFO_Ch1_DataAcquisition_Thread =
      m_DataAcquisition_Ch1.runInThread("FIFO_CH1_DAQ",
          std::bind( & OscilProcessing::FIFO_Ch1_DataAcquisition_thread_body, this));
}

void OscilProcessing::stop_DataAcquisition_Ch1() {
  m_bStop_FIFO_Ch1_DataAcquisition.store(true, std::memory_order_relaxed);
  m_FIFO_Ch1_DataAcquisition_Thread.join();
}

bool OscilProcessing::allow_DataAcquisition_Change_Ch1(const nds::state_t,
    const nds::state_t, 
    const nds::state_t) {
        return true;
}

//Thread for Channel 1 data from LabVIEW FPGA DMA FIFO
void OscilProcessing::FIFO_Ch1_DataAcquisition_thread_body() {
  NiFpga_Status FIFO_status;
  std::int16_t* pFIFO_Ch1_Data;
  std::vector<int32_t> FIFO_Ch1_Data;
  
  size_t NumberOfPushedDataBlocks_Ch1 = 0;
  size_t elements_requested_Ch1 = 2047;

  size_t FIFO_Ch1_DataAvailable = 0;

  while (!m_bStop_FIFO_Ch1_DataAcquisition.load(std::memory_order_relaxed)) {
    //Acquire all Target-to-Host DMA FIFOs
    //Channel 0 Data
    FIFO_status = NiFpga_AcquireFifoReadElementsI16(
      session,
      NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch1, 
      &pFIFO_Ch1_Data, 0, 0,
      &FIFO_Ch1_DataAcquired, 
      &FIFO_Ch1_DataAvailable);

    if (FIFO_Ch1_DataAvailable > 0) {
      FIFO_status = NiFpga_AcquireFifoReadElementsI16(
        session,
        NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch1, 
        &pFIFO_Ch1_Data, 
        FIFO_Ch1_DataAvailable, 0,
        &FIFO_Ch1_DataAcquired, 
        &FIFO_Ch1_DataAvailable);

      FIFO_Ch1_Data.insert(FIFO_Ch1_Data.end(),  pFIFO_Ch1_Data, pFIFO_Ch1_Data+FIFO_Ch1_DataAcquired);
      
      FIFO_status = NiFpga_ReleaseFifoElements(
        session, 
        NiFpga_NI5772_Oscilloscope_FPGA_Top_TargetToHostFifoI16_DMA_ToHost_Samples_Ch1, 
        FIFO_Ch1_DataAcquired);

      m_DataAcquisition_Ch1.push(m_DataAcquisition_Ch1.getTimestamp(), FIFO_Ch1_Data);
      ++NumberOfPushedDataBlocks_Ch1;
    }
    FIFO_Ch1_Data.clear();
  }
  m_DataAcquisition_Ch1.setNumberOfPushedDataBlocks(m_DataAcquisition_Ch1.getTimestamp(), NumberOfPushedDataBlocks_Ch1);
}

/*Channel 1*/
void OscilProcessing::PV_DataAcquisition_Gain_Writer_Ch1(const timespec & timestamp,
  const double & value) {
  double HW_value;
  HW_value = value;
  m_DataAcquisition_Ch1.setGain(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Offset_Writer_Ch1(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Offset to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Offset programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch1.setOffset(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Bandwidth_Writer_Ch1(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Bandwidth to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Bandwidth programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch1.setBandwidth(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Resolution_Writer_Ch1(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Resolution to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Resolution programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch1.setResolution(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Impedance_Writer_Ch1(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the Impedance to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Impedance programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch1.setImpedance(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Coupling_Writer_Ch1(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the Coupling to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Coupling programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch1.setCoupling(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_SignalRefType_Writer_Ch1(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the SignalRefType to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real SignalRefType programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch1.setSignalRefType(timestamp, HW_value);
}
void OscilProcessing::PV_DataAcquisition_Ground_Writer_Ch1(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the Ground to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Ground programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch1.setGround(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_DMAEnable_Writer_Ch1(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the DMAEnable value to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real DMAEnable value programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch1.setDMAEnable(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_SamplingRate_Writer_Ch1(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the SamplingRate to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real SamplingRate programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch1.setSamplingRate(timestamp, HW_value);
}

void OscilProcessing::PV_DataAcquisition_Gain_Initializer_Ch1(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    10
  };
  * value = 2.5; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Offset_Initializer_Ch1(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    20
  };
  * value = 3.0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Bandwidth_Initializer_Ch1(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    30
  };
  * value = 10.0; //Note that this value has no sense and it is fixed only for testing purposes.

}

void OscilProcessing::PV_DataAcquisition_Resolution_Initializer_Ch1(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    40
  };
  * value = 15.0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Impedance_Initializer_Ch1(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    50
  };
  * value = 2.2; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Coupling_Initializer_Ch1(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    60
  };
  * value = 1; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_SignalRefType_Initializer_Ch1(
  timespec * timestamp, int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    70
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_Ground_Initializer_Ch1(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    80
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_DMAEnable_Initializer_Ch1(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    90
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void OscilProcessing::PV_DataAcquisition_SamplingRate_Initializer_Ch1(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    100
  };
  * value = 10.0; //Note that this value has no sense and it is fixed only for testing purposes.
}