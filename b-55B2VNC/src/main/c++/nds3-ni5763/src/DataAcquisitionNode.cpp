#include "./../include/Ni5763Main.h"
#include <numeric>
#include <iostream>

#define NDS_EPOCH 1514764800

//DataAcquisitionNode StateMachine for channel 0
void Ni5763Main::switchOn_DataAcquisition_Ch0() {}
void Ni5763Main::switchOff_DataAcquisition_Ch0() {}
void Ni5763Main::recover_DataAcquisition_Ch0() {}

void Ni5763Main::start_DataAcquisition_Ch0() {
    m_bStop_DataAcquisition_Ch0.store(false, std::memory_order_relaxed);
    m_bStop_RawDataAcquisition_Ch0.store(false, std::memory_order_relaxed); //starts the cycle
    m_RawDataAcquisition_Ch0_Thread =
        m_DataAcquisition_Ch0.runInThread("RAW_DAQ_CH0",
            std::bind(&Ni5763Main::RawDataAcquisition_Ch0_thread_body, this));
    m_DataAcquisition_Ch0_Thread =
        m_DataAcquisition_Ch0.runInThread("DAQ_CH0",
            std::bind(&Ni5763Main::DataAcquisition_Ch0_thread_body, this)); // runs the thread in thread body method
}

void Ni5763Main::stop_DataAcquisition_Ch0() {
  m_bStop_RawDataAcquisition_Ch0.store(true, std::memory_order_relaxed);
  m_RawDataAcquisition_Ch0_Thread.join();
  m_bStop_DataAcquisition_Ch0.store(true, std::memory_order_relaxed);
  m_DataAcquisition_Ch0_Thread.join();
}

bool Ni5763Main::allow_DataAcquisition_Change_Ch0(const nds::state_t,
    const nds::state_t, 
    const nds::state_t) {
        return true;
}

//Averaging function for FluctCountRate
double Ni5763Main::count_average(std::vector<std::int64_t>& data_buffer, size_t size) {
  double average = 0.0;
  if (!data_buffer.empty()) {
    auto startIt = data_buffer.begin();
    auto endIt = startIt + size;

    double sum = std::accumulate(startIt, endIt, 0.0);
    average = sum / size;
    data_buffer.clear();
  } 
  else {

  }
  return average;
}

//Thread for Channel 0 data from LabVIEW FPGA DMA FIFO
void Ni5763Main::RawDataAcquisition_Ch0_thread_body() {
    NiFpga_Status RawData_status;
    std::int16_t* pFIFO_Ch0_RawData;
    std::vector<int32_t> FIFO_Ch0_RawData;
  
    size_t NumberOfPushedDataBlocks_Ch0 = 0;
    size_t elements_requested_Ch0 = 2047;

    size_t FIFO_Ch0_RawDataAvailable = 0;

    while (!m_bStop_RawDataAcquisition_Ch0.load(std::memory_order_relaxed)) {
        //Acquire all Target-to-Host DMA FIFOs
        //Channel 0 Data
        RawData_status = NiFpga_AcquireFifoReadElementsI16(
            session,
            NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16_RawData_ToHost_Ch0, 
            &pFIFO_Ch0_RawData, 0, 0,
            &RawData_DataAcquired, 
            &FIFO_Ch0_RawDataAvailable);
        
        if (FIFO_Ch0_RawDataAvailable > 0) {
            RawData_status = NiFpga_AcquireFifoReadElementsI16(
                session,
                NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16_RawData_ToHost_Ch0, 
                &pFIFO_Ch0_RawData, 
                FIFO_Ch0_RawDataAvailable, 0,
                &RawData_DataAcquired, 
                &FIFO_Ch0_RawDataAvailable);

            FIFO_Ch0_RawData.insert(FIFO_Ch0_RawData.end(),  pFIFO_Ch0_RawData, pFIFO_Ch0_RawData+RawData_DataAcquired);
      
            RawData_status = NiFpga_ReleaseFifoElements(
                session, 
                NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoI16_RawData_ToHost_Ch0, 
                RawData_DataAcquired);

            m_DataAcquisition_Ch0.push(m_DataAcquisition_Ch0.getTimestamp(), FIFO_Ch0_RawData);
            ++NumberOfPushedDataBlocks_Ch0;
        }
    FIFO_Ch0_RawData.clear();
  }
  m_DataAcquisition_Ch0.setNumberOfPushedDataBlocks(m_DataAcquisition_Ch0.getTimestamp(), NumberOfPushedDataBlocks_Ch0);
}

void Ni5763Main::DataAcquisition_Ch0_thread_body() {
  NiFpga_Status FIFO_status;

  std::uint32_t* pFluctCountRate_Hz_Data;
  std::uint32_t* pPulseChain_Data;

  std::vector<int64_t> FluctCountRate_Hz_Data;
  std::vector<int64_t> PulseChain_Data;
  std::vector<double> SingleRate_Data;
  std::vector<double> TotalRate_Data;

  size_t NumberOfPushedDataBlocks_Ch0 = 0;
  size_t elements_requested_Ch0 = 2047;

  size_t FluctCountRate_Hz_DataAvailable = 0;
  size_t PulseChain_DataAvailable = 0;

  while (!m_bStop_DataAcquisition_Ch0.load(std::memory_order_relaxed)) {
    //Acquire all Target-to-Host DMA FIFOs
    //Baseline FIFO
    //FluctCountRate_Hz FIFO
    FIFO_status = NiFpga_AcquireFifoReadElementsU32(
        session,
        NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_FluctCountRate_Hz_ToHost_Ch0, 
        &pFluctCountRate_Hz_Data, 0, 0,
        &FluctCountRate_Hz_DataAcquired, 
        &FluctCountRate_Hz_DataAvailable);

    if (FluctCountRate_Hz_DataAvailable > 0) {
      FIFO_status = NiFpga_AcquireFifoReadElementsU32(
          session,
          NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_FluctCountRate_Hz_ToHost_Ch0, 
          &pFluctCountRate_Hz_Data, 
          FluctCountRate_Hz_DataAvailable, 0,
          &FluctCountRate_Hz_DataAcquired, 
          &FluctCountRate_Hz_DataAvailable);

      FluctCountRate_Hz_Data.insert(FluctCountRate_Hz_Data.end(),  pFluctCountRate_Hz_Data, pFluctCountRate_Hz_Data + FluctCountRate_Hz_DataAcquired);

      FIFO_status = NiFpga_ReleaseFifoElements(
          session, 
          NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_FluctCountRate_Hz_ToHost_Ch0, 
          FluctCountRate_Hz_DataAcquired);

      FluctCountRate_Hz_Average = count_average(FluctCountRate_Hz_Data, FluctCountRate_Hz_Data.size());

      FluctCountRate_Hz_m.push(m_DataAcquisition_Ch0.getTimestamp(), FluctCountRate_Hz_Average);
      ++NumberOfPushedDataBlocks_Ch0;
    }
    FluctCountRate_Hz_Data.clear();

    //PulseChain FIFO
    FIFO_status = NiFpga_AcquireFifoReadElementsU32(
        session,
        NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_PulseChain_ToHost_Ch0, 
        &pPulseChain_Data, 0, 0,
        &PulseChain_DataAcquired, 
        &PulseChain_DataAvailable);

    if (PulseChain_DataAvailable > 0) {
      FIFO_status = NiFpga_AcquireFifoReadElementsU32(
          session,
          NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_PulseChain_ToHost_Ch0, 
          &pPulseChain_Data, 
          PulseChain_DataAvailable, 0,
          &PulseChain_DataAcquired, 
          &PulseChain_DataAvailable);

      PulseChain_Data.insert(PulseChain_Data.end(),  pPulseChain_Data, pPulseChain_Data + PulseChain_DataAcquired);

      FIFO_status = NiFpga_ReleaseFifoElements(
          session, 
          NiFpga_Fission_Chamber_DAQ_FPGA_Top_TargetToHostFifoU32_PulseChain_ToHost_Ch0, 
          PulseChain_DataAcquired);

      SinglePules_m.push(m_DataAcquisition_Ch0.getTimestamp(), PulseChain_Data[0]);
      SingleRate_m.push(m_DataAcquisition_Ch0.getTimestamp(), static_cast<double>(PulseChain_Data[1]) * 0.001);
      TotalPulses_m.push(m_DataAcquisition_Ch0.getTimestamp(), PulseChain_Data[2]);
      TotalRate_m.push(m_DataAcquisition_Ch0.getTimestamp(), static_cast<double>(PulseChain_Data[3]) * 0.001);
      
      for(int i = 0; i < (PulseChain_Data.size() * 0.125); i += 4) {
        if((i+3) < PulseChain_Data.size()) { //check if the last elements exist
          SingleRate_Data.push_back(static_cast<double>(PulseChain_Data[i+1] * 0.001));
          TotalRate_Data.push_back(static_cast<double>(PulseChain_Data[i+3] * 0.001));
        }
      }
      SingleRateData_m.push(m_DataAcquisition_Ch0.getTimestamp(), SingleRate_Data);
      TotalRateData_m.push(m_DataAcquisition_Ch0.getTimestamp(), TotalRate_Data);
      ++NumberOfPushedDataBlocks_Ch0;
    }
    PulseChain_Data.clear();
    SingleRate_Data.clear();
    TotalRate_Data.clear();
  }
  m_DataAcquisition_Ch0.setNumberOfPushedDataBlocks(m_DataAcquisition_Ch0.getTimestamp(), NumberOfPushedDataBlocks_Ch0);
  
  FluctCountRate_Hz_DataAcquired = 0;
  PulseChain_DataAcquired = 0; 
}


/*Channel 0*/
void Ni5763Main::PV_DataAcquisition_Gain_Writer_Ch0(const timespec & timestamp,
  const double & value) {
    double HW_value;
    HW_value = value;
    m_DataAcquisition_Ch0.setGain(timestamp, HW_value);
}

void Ni5763Main::PV_DataAcquisition_Offset_Writer_Ch0(const timespec & timestamp,
  const double & value) {
    double HW_value;
    //Value has the Offset to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real Offset programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value = value;
    m_DataAcquisition_Ch0.setOffset(timestamp, HW_value);
}

void Ni5763Main::PV_DataAcquisition_Bandwidth_Writer_Ch0(const timespec & timestamp,
  const double & value) {
    double HW_value;
    //Value has the Bandwidth to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real Bandwidth programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value = value;
    m_DataAcquisition_Ch0.setBandwidth(timestamp, HW_value);
}

void Ni5763Main::PV_DataAcquisition_Resolution_Writer_Ch0(const timespec & timestamp,
  const double & value) {
    double HW_value;
    //Value has the Resolution to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real Resolution programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value = value;
    m_DataAcquisition_Ch0.setResolution(timestamp, HW_value);
}

void Ni5763Main::PV_DataAcquisition_Impedance_Writer_Ch0(const timespec & timestamp,
  const double & value) {
    double HW_value;
    //Value has the Impedance to be programmed on the hardware.
    //Call to function programming the hardware. This function should return the real Impedance programmed. This value has to be set to the readback attribute.
    //In the meantime, without real hardware value and  HW_value are equal.
    HW_value = value;
    m_DataAcquisition_Ch0.setImpedance(timestamp, HW_value);
}

void Ni5763Main::PV_DataAcquisition_Coupling_Writer_Ch0(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the Coupling to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Coupling programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setCoupling(timestamp, HW_value);
}

void Ni5763Main::PV_DataAcquisition_SignalRefType_Writer_Ch0(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the SignalRefType to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real SignalRefType programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setSignalRefType(timestamp, HW_value);
}
void Ni5763Main::PV_DataAcquisition_Ground_Writer_Ch0(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the Ground to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real Ground programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setGround(timestamp, HW_value);
}

void Ni5763Main::PV_DataAcquisition_DMAEnable_Writer_Ch0(const timespec & timestamp,
  const std::int32_t & value) {
  std::int32_t HW_value;
  //Value has the DMAEnable value to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real DMAEnable value programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setDMAEnable(timestamp, HW_value);
}

void Ni5763Main::PV_DataAcquisition_SamplingRate_Writer_Ch0(const timespec & timestamp,
  const double & value) {
  double HW_value;
  //Value has the SamplingRate to be programmed on the hardware.
  //Call to function programming the hardware. This function should return the real SamplingRate programmed. This value has to be set to the readback attribute.
  //In the meantime, without real hardware value and  HW_value are equal.
  HW_value = value;
  m_DataAcquisition_Ch0.setSamplingRate(timestamp, HW_value);
}

void Ni5763Main::PV_DataAcquisition_Gain_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    10
  };
  * value = 2.5; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5763Main::PV_DataAcquisition_Offset_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    20
  };
  * value = 3.0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5763Main::PV_DataAcquisition_Bandwidth_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    30
  };
  * value = 10.0; //Note that this value has no sense and it is fixed only for testing purposes.

}

void Ni5763Main::PV_DataAcquisition_Resolution_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    40
  };
  * value = 15.0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5763Main::PV_DataAcquisition_Impedance_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    50
  };
  * value = 2.2; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5763Main::PV_DataAcquisition_Coupling_Initializer_Ch0(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    60
  };
  * value = 1; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5763Main::PV_DataAcquisition_SignalRefType_Initializer_Ch0(
  timespec * timestamp, int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    70
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5763Main::PV_DataAcquisition_Ground_Initializer_Ch0(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    80
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5763Main::PV_DataAcquisition_DMAEnable_Initializer_Ch0(timespec * timestamp,
  int32_t * value) {
  * timestamp = {
    NDS_EPOCH,
    90
  };
  * value = 0; //Note that this value has no sense and it is fixed only for testing purposes.
}

void Ni5763Main::PV_DataAcquisition_SamplingRate_Initializer_Ch0(timespec * timestamp,
  double * value) {
  * timestamp = {
    NDS_EPOCH,
    100
  };
  * value = 10.0; //Note that this value has no sense and it is fixed only for testing purposes.
}